#include "DaisyDuino.h"

#define SINGLE_MAX_SIZE (48000 * 60 * 1) // 1 minute of floats at 48 khz
#define TOTAL_MAX_SIZE SINGLE_MAX_SIZE * 3 // 3 loops

static DaisyHardware petal;

enum State {
  idle_empty,
  armed,
  rec_first,
  play,
  overdub,
  idle
};
State loop1State;
State loop2State;
State loop3State;

int pos = 0;
float DSY_SDRAM_BSS buf[TOTAL_MAX_SIZE];
int mod = SINGLE_MAX_SIZE;
int len = 0;
float drywet = 0.5;
bool res = false;



  

void ResetBuffer();
void Controls();

void NextSamples(float &output, float *in, size_t i);

static void AudioCallback(float *in, float *out, size_t size) {
  float output = 0;
  
  Controls();
  float max_value = 0.0;
  float min_value = 1.0;
  for (size_t i = 0; i < size; i += 2) {
    NextSamples(output, in, i);

    // left and right outs
    out[i] = out[i + 1] = output;
    if (output>max_value) max_value=output;
    if (output<min_value) min_value=output;
    if ((max_value-min_value)*(16777216)>50000) {
      if (loop1State==armed) {
        loop1State = rec_first;
      }
    }
  }



}

void setup() {
  // initialize petal hardware and oscillator daisysp module

  petal = DAISY.init(DAISY_PETAL, AUDIO_SR_48K);
  ResetBuffer();
  loop1State = idle_empty;


  petal.ClearLeds();

  // start callback
  DAISY.begin(AudioCallback);

  pinMode(22, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(0, OUTPUT);
  Serial.begin(115200);
}
void loop() {
  // leds
  //petal.SetFootswitchLed(1, play);
  //petal.SetFootswitchLed(0, rec);
  //petal.UpdateLeds();
  //analogWrite(22, petal.GetKnobValue(1)* 255);
  //Serial.println((int)(petal.GetKnobValue(1)* 255));
  if (loop1State==idle_empty) {
    Serial.println("idle_empty");
  } else if (loop1State==armed) {
    Serial.println("armed");
  } else if (loop1State==rec_first) {
    Serial.println("rec_first");
  } else if (loop1State==play) {
    Serial.println("play");
  } else if (loop1State==overdub) {
    Serial.println("overdub");
  } else if (loop1State==idle) {
    Serial.println("idle");
  }
  Serial.print("mod: ");
  Serial.print(mod);
  Serial.print(", pos: ");
  Serial.print(pos);
  Serial.print(", len: ");
  Serial.println(len);

  digitalWrite(22, (loop1State==armed));
  digitalWrite(1, (loop1State==play));
  digitalWrite(0, ((loop1State == rec_first) || (loop1State == overdub)));
  //digitalWrite(22, (max_counts>5000));
  //digitalWrite(1, (max_counts>50000));
  //digitalWrite(0, (max_counts>500000));
  delay(200);
}

void printStatus(State state, int name) {

}
// Resets the buffer
void ResetBuffer() {
  pos = 0;
  len = 0;
  for (int i = 0; i < mod; i++) {
    buf[i] = 0;
  }
  mod = SINGLE_MAX_SIZE;
}

void UpdateButtons() {
  // switch1 pressed
  if (petal.buttons[0].RisingEdge()) { // pressed play button
    if (loop1State == rec_first) {
      mod = len;
      len = 0;
      Serial.print("Rec->Play: ");
      Serial.println(mod);
      loop1State = play;
    } else if (loop1State == overdub) {
      loop1State = play;
    }else if (loop1State == play) {
      loop1State = idle;
    }else if (loop1State == idle) {
      loop1State = play;
    }
  }

  // switch1 held
  if (petal.buttons[0].TimeHeldMs() >= 1000 && loop1State != idle_empty) { // hold play button
    ResetBuffer();
    loop1State = idle_empty;
  }

  // switch2 pressed and not empty buffer
  if (petal.buttons[1].RisingEdge()) { // pres rec button
    if (loop1State == idle_empty) {
      loop1State = armed;
    } else if (loop1State == rec_first) {
      mod = len;
      len = 0;
      loop1State = overdub;
    } else if (loop1State == play) {
      loop1State = overdub;
    }else if (loop1State == idle) {
      loop1State = overdub;
    }
  }

}

// Deals with analog controls
void Controls() {
  petal.ProcessAllControls();
  
  drywet = petal.controls[0].Value();

  UpdateButtons();
}

void WriteBuffer(float *in, size_t i) {
  buf[pos] = buf[pos] * 0.5 + in[i] * 0.5;
  if (loop1State == rec_first) {
    len++;
  }
}

void NextSamples(float &output, float *in, size_t i) {
  if ((loop1State == rec_first) || (loop1State == overdub)) {
    //WriteBuffer(in, i);
    buf[pos] = buf[pos] * 1.0 + in[i] * 1.0;
    if (loop1State == rec_first) {
     len++;
    }
  }

  output = buf[pos];

  // automatic looptime
  if (len >= SINGLE_MAX_SIZE) {
    mod = SINGLE_MAX_SIZE;
    len = 0;
    loop1State=play;
  }

  if ((loop1State == rec_first) || (loop1State == play) || (loop1State == overdub)) {
    pos++;
    pos %= mod;
  }

  if ((loop1State != rec_first) && (loop1State != overdub)) {
    output = output * 1.0 + in[i] * 1.0;
  }
}