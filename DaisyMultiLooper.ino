#include "DaisyDuino.h"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#define DISPLAY_ADDRESS 0x70

#define SINGLE_MAX_SIZE (48000 * 20) // 20 seconds of floats at 48 khz
#define LOOP2OFFSET (SINGLE_MAX_SIZE)
#define LOOP3OFFSET (SINGLE_MAX_SIZE * 2)
#define TOTAL_MAX_SIZE (SINGLE_MAX_SIZE * 3) // 3 loops

Adafruit_AlphaNum4 disp = Adafruit_AlphaNum4();
int animloop = 0;

static DaisyHardware petal;

enum State {
  idle_empty,
  armed,
  rec_first,
  play, // used in all loops
  overdub, // used in all loops
  idle // used in all loops
};
State loop1State = idle_empty;
State loop2State = idle;
State loop3State = idle;

int pos = 0;
float DSY_SDRAM_BSS buf[TOTAL_MAX_SIZE];
int mod = SINGLE_MAX_SIZE;
int len = 0;
  

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
    if ((max_value-min_value)*(16777216)>70000) {
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

  // start callback
  DAISY.begin(AudioCallback);

  pinMode(22, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(0, OUTPUT);
  Serial.begin(115200);

  // initialize 4 digit LED alpha display
  disp.begin(DISPLAY_ADDRESS);
  disp.setBrightness(4);
  disp.clear();
  disp.writeDisplay();
}

void loop() {

  if (petal.buttons[0].Pressed()) Serial.print("X"); else Serial.print("_");
  if (petal.buttons[1].Pressed()) Serial.print("X"); else Serial.print("_");
  if (petal.buttons[2].Pressed()) Serial.print("X"); else Serial.print("_");
  if (petal.buttons[3].Pressed()) Serial.print("X"); else Serial.print("_");
  if (petal.buttons[4].Pressed()) Serial.print("X"); else Serial.print("_");
  if (petal.buttons[5].Pressed()) Serial.print("X"); else Serial.print("_");
  if (petal.buttons[6].Pressed()) Serial.print("X"); else Serial.print("_");
  Serial.println(" <- buttons");
  printStatus(loop1State, 1);
  printStatus(loop2State, 2);
  printStatus(loop3State, 3);
  Serial.print("mod: ");
  Serial.print(mod);
  Serial.print(", pos: ");
  Serial.print(pos);
  Serial.print(", len: ");
  Serial.println(len);

  digitalWrite(22, (loop1State==armed));
  digitalWrite(1, (loop1State==play));
  digitalWrite(0, ((loop1State == rec_first) || (loop1State == overdub)));

  animloop++;
  if (animloop==4) animloop = 0;
  if (loop1State==armed) {
    disp.writeDigitRaw(3, 0b111111);
    disp.writeDigitRaw(2, 0b1000);
    disp.writeDigitRaw(1, 0b1000);
    disp.writeDigitRaw(0, 0b111111);
  } else if (loop1State == rec_first) {
    switch (animloop) {
      case 0:
        disp.writeDigitRaw(3, 0b11111111);
        disp.writeDigitRaw(0, 0b01001000111111);
        break;
      case 1:
        disp.writeDigitRaw(3, 0b10000100111111);
        disp.writeDigitRaw(0, 0b00110000111111);
        break;
      case 2:
        disp.writeDigitRaw(3, 0b01001000111111);
        disp.writeDigitRaw(0, 0b11111111);
        break;
      case 3:
        disp.writeDigitRaw(3, 0b00110000111111);
        disp.writeDigitRaw(0, 0b10000100111111);
        break;
    }
    disp.writeDigitRaw(2, 0b1000);
    disp.writeDigitRaw(1, 0b1000);
  } else {
    if (loop1State==play) {
      disp.writeDigitRaw(3, 0b10010000000110);
    } else if (loop1State==overdub) {
      disp.writeDigitRaw(3, 0b10110100110110);
    } else {
      disp.writeDigitRaw(3, 0b0);
    }
    if (loop2State==play) {
      disp.writeDigitRaw(2, 0b10010000000110);
    } else if (loop2State==overdub) {
      disp.writeDigitRaw(2, 0b10110100110110);
    } else {
      disp.writeDigitRaw(2, 0b0);
    }
    if (loop3State==play) {
      disp.writeDigitRaw(1, 0b10010000000110);
    } else if (loop3State==overdub) {
      disp.writeDigitRaw(1, 0b10110100110110);
    } else {
      disp.writeDigitRaw(1, 0b0);
    }
    disp.writeDigitRaw(0, 0b0);
  }

  disp.writeDisplay();

  delay(200);
}

void printStatus(State state, int name) {
  Serial.print(name);
  if (state==idle_empty) {
    Serial.println(": idle_empty");
  } else if (state==armed) {
    Serial.println(": armed");
  } else if (state==rec_first) {
    Serial.println(": rec_first");
  } else if (state==play) {
    Serial.println(": play");
  } else if (state==overdub) {
    Serial.println(": overdub");
  } else if (state==idle) {
    Serial.println(": idle");
  }
}
// Resets the buffer
void ResetBuffer() {
  pos = 0;
  len = 0;
  for (int i = 0; i < TOTAL_MAX_SIZE; i++) {
    buf[i] = 0;
  }
  mod = SINGLE_MAX_SIZE;
}

void UpdateButtons() {
  if (petal.buttons[1].RisingEdge()) { // loop 1 rec
    if (loop1State == idle_empty) {
      loop1State = armed;
    } else if (loop1State == rec_first) {
      mod = len;
      len = 0;
      loop1State = overdub;
    } else if (loop1State == play) {
      loop1State = overdub;
    } else if (loop1State == idle) {
      loop1State = overdub;
    } else if (loop1State == overdub) {
      loop1State = play;
    }
    if (loop2State == overdub) loop2State = play; // downgrade any recording loops to playing
    if (loop3State == overdub) loop3State = play;
  }
  if (petal.buttons[3].RisingEdge()) { // loop 2 rec
    if ((loop2State == idle) || (loop2State == play)) {
      loop2State = overdub;
    } else if (loop2State == overdub) {
      loop2State = play;
    }
    if (loop1State == overdub) loop1State = play; // downgrade any recording loops to playing
    if (loop3State == overdub) loop3State = play;
  }
  if (petal.buttons[5].RisingEdge()) { // loop 3 rec
    if ((loop3State == idle) || (loop3State == play)) {
      loop3State = overdub;
    } else if (loop3State == overdub) {
      loop3State = play;
    }
    if (loop1State == overdub) loop1State = play; // downgrade any recording loops to playing
    if (loop2State == overdub) loop2State = play;
  }



  // switch1 pressed
  if (petal.buttons[0].RisingEdge()) { // pressed loop 1 play button
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
  if (petal.buttons[2].RisingEdge()) { // pressed loop 2 play button
    if (loop2State == overdub) {
      loop2State = play;
    }else if (loop2State == play) {
      loop2State = idle;
    }else if (loop2State == idle) {
      loop2State = play;
    }
  }
  if (petal.buttons[4].RisingEdge()) { // pressed loop 3 play button
    if (loop3State == overdub) {
      loop3State = play;
    }else if (loop3State == play) {
      loop3State = idle;
    }else if (loop3State == idle) {
      loop3State = play;
    }
  }

  if (petal.buttons[0].TimeHeldMs() >= 1000 && loop1State != idle_empty) { // hold play button
    ResetBuffer();
    loop1State = idle_empty;
    loop2State = idle;
    loop3State = idle;
  }

  if ((loop1State == idle) && (loop2State == idle) && (loop3State == idle)) pos = 0; // if nothing else is playing, move playback head to start
}

// Deals with analog controls
void Controls() {
  petal.ProcessAllControls();
  UpdateButtons();
}

void NextSamples(float &output, float *in, size_t i) {
  // Mirror input to output
  output = in[i];
  // add each playback to output before recording new layers
  if (loop1State!=idle) output = output + buf[pos];
  if (loop2State!=idle) output = output + buf[LOOP2OFFSET + pos];
  if (loop3State!=idle) output = output + buf[LOOP3OFFSET + pos];
  // record new layers if recording active for that looper
  if ((loop1State == rec_first) || (loop1State == overdub)) buf[pos] = buf[pos] + in[i];
  if (loop2State == overdub) buf[LOOP2OFFSET + pos] = buf[LOOP2OFFSET + pos] + in[i];
  if (loop3State == overdub) buf[LOOP3OFFSET + pos] = buf[LOOP3OFFSET + pos] + in[i];
  // increment looptime if this is the first layer
  if (loop1State == rec_first) len++;

  // automatic looptime
  if (len >= SINGLE_MAX_SIZE) {
    mod = SINGLE_MAX_SIZE;
    len = 0;
    loop1State=play;
  }
  // if anything is playing or recording increment position
  if ((loop1State == rec_first) || (loop1State == play) || (loop1State == overdub)
        || (loop2State == play) || (loop2State == overdub)
        || (loop3State == play) || (loop3State == overdub)) {
    pos++;
    pos %= mod;
  }
}