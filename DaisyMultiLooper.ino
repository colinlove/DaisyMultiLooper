#include "DaisyDuino.h"
#include "DaisyMultiLooper.h"
#include "Font.h"

#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
#define DISPLAY_ADDRESS 112
#define ECHO_SAMPLE_MAX (2*48000) // second @ 48KHz
#define VERB_SAMPLE_MAX (1*48000) // second @ 48KHz

Adafruit_AlphaNum4 disp[8];

static DaisyHardware petal;

unsigned long lastStartTime;
unsigned long startTime;
unsigned long stopTime;
int timeCalcLoop=0;

int input_chan=0;

float vu_in_max=0;
float vu_out_max=0;

int inp_dist=0;
int last_dist=2;
int inp_verb=0;
int last_verb=2;
int inp_echo=0;
int last_echo=2;
int inp_trem=0;
int last_trem=2;

int inp_mell=0;

// distortion
  float dist_SoftGain = 10;
  float dist_HardGain = 10;
  float dist_OutGain = 0.03;
// tremolo
  float trem_mag = 0.65; // max 1
  float trem_period = 0.25;
  float trem_time = 0;
  float trem_scale = 0; // temp
// echo
  float DSY_SDRAM_BSS echo_buf[ECHO_SAMPLE_MAX];
  int echo_period=12000;
  int tap_period=-1;
  int echo_loop_fraction=-2;
  int echo_time=0;
  int echo_valid_samples=0;
  float echo_feedback=0.25;
  float echo_wet=0.3;
// reverb
  float DSY_SDRAM_BSS verb_buf[VERB_SAMPLE_MAX];
  int verb_time=0;
  int verb_valid_samples=0;
  int verb_period=1000;
  float verb_feedback=0.5;
  float verb_wet=0.3;
// loop
#define SINGLE_MAX_SIZE (48000 * 20) // 20 seconds of floats at 48 khz
#define LOOP2OFFSET (SINGLE_MAX_SIZE)
#define LOOP3OFFSET (SINGLE_MAX_SIZE * 2)
#define LOOP4OFFSET (SINGLE_MAX_SIZE * 3)
#define TOTAL_MAX_SIZE (SINGLE_MAX_SIZE * 4) // 4 loops



int pos = 0;
float DSY_SDRAM_BSS buf[TOTAL_MAX_SIZE];
int mod = SINGLE_MAX_SIZE;
int len = 0;

int animloop = 0;

ChannelState loop1State, loop2State, loop3State, loop4State;

enum UIStateType {
  UI_Welcome,
  UI_Anim,

  UI_FX1, // shows all different FX
  UI_FX2, // shows all different FX

  UI_Dist, // distortion sub-menu
  UI_Verb, // Reverb sub-menu
  UI_Echo, // Echo sub-menu
  UI_Trem, // Trem sub-menu

  UI_Comp, // Compression sub-menu
  UI_Octv, // Octave sub-menu
  UI_Mell, // Mellow sub-menu
  UI_Chor, // Chorus sub-menu

  UI_Loop_Play_Rec, // shows loop status and play/rec buttons
  UI_Loop_Options,

  UI_Loop1, // shows extra loop 1 options
  UI_Loop2, // shows extra loop 2 options
  UI_Loop3, // shows extra loop 3 options
  UI_Loop4, // shows extra loop 4 options

  UI_VU, // Volume meter

  UI_BOOT, // used when put into bootloader mode
  UI_xx
};
UIStateType UI = UI_Anim;

void setup() {
  for (int i=0; i<8; i++) {
    disp[i] = Adafruit_AlphaNum4();
    disp[i].begin(DISPLAY_ADDRESS+i);
    disp[i].setBrightness(1);
    disp[i].clear();
    disp[i].writeDisplay();
  }
  petal = DAISY.init(DAISY_PETAL, AUDIO_SR_48K);
  DAISY.SetAudioBlockSize(48); // default is 48 samples for 1ms blocksize
  // loop setup
    ResetBuffer();
    loop1State = idle_empty;
    loop2State = idle_empty;
    loop3State = idle_empty;
    loop4State = idle_empty;

    for (int i=0; i<ECHO_SAMPLE_MAX; i++) echo_buf[i]=0;
    for (int i=0; i<VERB_SAMPLE_MAX; i++) verb_buf[i]=0;
  //------------------------------------------------------------------------------------------------------------
  DAISY.begin(AudioCallback);
  //------------------------------------------------------------------------------------------------------------

  Serial.begin(115200);

}
static void AudioCallback(float **in, float **out, size_t size) {
  stopTime = micros();
  petal.ProcessAllControls();
  UI_Menu_Buttons();
  float trem_timescale=2*PI/trem_period;
  float max_value = 0.0;
  float min_value = 1.0;
  for (size_t i = 0; i < size; i++) {
    float signal = (input_chan==0)? in[0][i]:in[1][i];
    if (signal>vu_in_max) vu_in_max=signal;
    //vu_in_max=signal;
    //if (-signal>vu_in_max) vu_in_max=-signal;
    // use dry signal for threshold measurement
    if (signal>max_value) max_value=signal;
    if (signal<min_value) min_value=signal;
    if ((max_value-min_value)*(16777216)>150000) {
      if (loop1State==armed) {
        loop1State = rec_first;
      }
    }

    if (inp_dist) {
      signal *= dist_SoftGain;
      signal = softClip(signal);
      signal *= dist_HardGain;
      signal = hardClip(signal);
      signal *= dist_OutGain;
    }

    if (inp_trem) {
      trem_time+=(1.0/48000.0);
      //if (trem_time>trem_period) trem_time-=trem_period;
      trem_scale = 1 + (sin(trem_time*trem_timescale)*trem_mag);
      signal *= trem_scale;
    }

    if (inp_echo) {
      float buffer_in = (echo_valid_samples>echo_time)? echo_buf[echo_time] : 0;

      echo_buf[echo_time] = signal + (buffer_in * echo_feedback);
      signal+=buffer_in*echo_wet;

      if (echo_valid_samples<=echo_period) echo_valid_samples++;
      echo_time++;
      if (echo_time>=echo_period) echo_time=0;
    } else {
      echo_time=echo_valid_samples=0;
    }

    if (tap_period>=0) tap_period++;
    if (tap_period>=ECHO_SAMPLE_MAX) tap_period=-1;

    if (inp_verb) {
      float buffer_in = (verb_valid_samples>verb_time)? verb_buf[verb_time] : 0;

      verb_buf[verb_time] = signal + (buffer_in * verb_feedback);
      signal+=buffer_in*verb_wet;

      if (verb_valid_samples<=verb_period) verb_valid_samples++;
      verb_time++;
      if (verb_time>=verb_period) verb_time=0;
    } else {
      verb_time=verb_valid_samples=0;
    }

    // Mirror input to output
    float signal_in=signal;
    // add each playback to output before recording new layers
    if (loop1State!=idle) signal += buf[pos];
    if (loop2State!=idle) signal += buf[LOOP2OFFSET + pos];
    if (loop3State!=idle) signal += buf[LOOP3OFFSET + pos];
    if (loop4State!=idle) signal += buf[LOOP4OFFSET + pos];
    // record new layers if recording active for that looper
    if ((loop1State == rec_first) || (loop1State == overdub)) buf[pos] += signal_in;
    if (loop2State == overdub) buf[LOOP2OFFSET + pos] += signal_in;
    if (loop3State == overdub) buf[LOOP3OFFSET + pos] += signal_in;
    if (loop4State == overdub) buf[LOOP4OFFSET + pos] += signal_in;
    // increment looptime if this is the first layer
    if (loop1State == rec_first) len++;

    // automatic looptime
    if (len >= SINGLE_MAX_SIZE) {
      mod = SINGLE_MAX_SIZE;
      len = 0;
      loop1State=play;
    }
    // if anything is playing or recording then increment position
    if ((loop1State == rec_first) || (loop1State == play) || (loop1State == overdub)
          || (loop2State == play) || (loop2State == overdub)
          || (loop3State == play) || (loop3State == overdub)
          || (loop4State == play) || (loop4State == overdub)) {
      pos++;
      pos %= mod;
    }
    if (signal>vu_out_max) vu_out_max=signal;
    if (-signal>vu_out_max) vu_out_max=-signal;

    if (inp_mell) signal=0;
    signal*=4;
    out[0][i] = out[1][i] = signal;


  }
  lastStartTime = startTime;
  startTime = micros();
}
void loop() {

  /*
  if (petal.buttons[0].Pressed()) Serial.print("X"); else Serial.print("_");
  if (petal.buttons[1].Pressed()) Serial.print("X"); else Serial.print("_");
  if (petal.buttons[2].Pressed()) Serial.print("X"); else Serial.print("_");
  if (petal.buttons[3].Pressed()) Serial.print("X"); else Serial.print("_");
  if (petal.buttons[4].Pressed()) Serial.print("X"); else Serial.print("_");
  if (petal.buttons[5].Pressed()) Serial.print("X"); else Serial.print("_");
  if (petal.buttons[6].Pressed()) Serial.print("X"); else Serial.print("_");
  if (petal.buttons[7].Pressed()) Serial.print("X"); else Serial.print("_");
  if (petal.buttons[8].Pressed()) Serial.print("X"); else Serial.print("_");
  if (petal.buttons[9].Pressed()) Serial.print("X"); else Serial.print("_");
  Serial.println(" <- buttons");
  */
  UI_Display();

  timeCalcLoop++;
  if (timeCalcLoop>100) {
    
    Serial.print("Interval: ");
    Serial.println(startTime-lastStartTime);
    Serial.print("Execution time: ");
    Serial.println(stopTime-startTime);
    
    timeCalcLoop=0;
  }

}


float hardClip(float in) {
  in = in > 1.f ? 1.f : in;
  in = in < -1.f ? -1.f : in;
  return in;
}

float softClip(float in) {
  if (in > 0)
    return 1 - expf(-in);
  return -1 + expf(in);
}