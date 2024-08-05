#define SINGLE_MAX_SIZE (48000 * 20) // 20 seconds of floats at 48 khz
#define LOOP2OFFSET (SINGLE_MAX_SIZE)
#define LOOP3OFFSET (SINGLE_MAX_SIZE * 2)
#define LOOP4OFFSET (SINGLE_MAX_SIZE * 3)
#define TOTAL_MAX_SIZE (SINGLE_MAX_SIZE * 4) // 4 loops

int animloop = 0;

enum ChannelState {
  idle_empty,
  armed,
  rec_first,
  play, // used in all loops
  overdub, // used in all loops
  idle // used in all loops
};
ChannelState loop1State, loop2State, loop3State, loop4State;

int pos = 0;
float DSY_SDRAM_BSS buf[TOTAL_MAX_SIZE];
int mod = SINGLE_MAX_SIZE;
int len = 0;
uint16_t time_marker;

void ResetBuffer();
void Controls();

void NextSamples(float &output, float *in, size_t i);

static void loop_AudioCallback(float *in, float *out, size_t size) {
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

void loop_setup() {
  ResetBuffer();
    loop1State = idle_empty;
    loop2State = idle_empty;
    loop3State = idle_empty;
    loop4State = idle_empty;
}

void loop_loop() {
  animloop++;
  if (animloop==4) animloop = 0;
  if (loop1State==idle_empty) {
    disp[0].writeDigitRaw(3, 0b111);
    disp[0].writeDigitRaw(2, 0b111111);
    disp[0].writeDigitRaw(1, 0b111111);
    disp[0].writeDigitRaw(0, 0b100010001110);
  } else if (loop1State==armed) {
    disp[0].writeDigitRaw(3, 0b111111);
    disp[0].writeDigitRaw(2, 0b1000);
    disp[0].writeDigitRaw(1, 0b1000);
    disp[0].writeDigitRaw(0, 0b111111);
  } else if (loop1State == rec_first) {
    switch (animloop) {
      case 0:
        disp[0].writeDigitRaw(3, 0b11111111);
        disp[0].writeDigitRaw(0, 0b01001000111111);
        break;
      case 1:
        disp[0].writeDigitRaw(3, 0b10000100111111);
        disp[0].writeDigitRaw(0, 0b00110000111111);
        break;
      case 2:
        disp[0].writeDigitRaw(3, 0b01001000111111);
        disp[0].writeDigitRaw(0, 0b11111111);
        break;
      case 3:
        disp[0].writeDigitRaw(3, 0b00110000111111);
        disp[0].writeDigitRaw(0, 0b10000100111111);
        break;
    }
    disp[0].writeDigitRaw(2, 0b1000);
    disp[0].writeDigitRaw(1, 0b1000);
  } else {
    if (pos<mod/8) {
      time_marker = 0b1000000000000;
    } else if (pos<mod*2/8) {
      time_marker = 0b1100000000000;
    } else if (pos<mod*3/8) {
      time_marker = 0b1100001000000;
    } else if (pos<mod*4/8) {
      time_marker = 0b1100101000000;
    } else if (pos<mod*5/8) {
      time_marker = 0b1101101000000;
    } else if (pos<mod*6/8) {
      time_marker = 0b1111101000000;
    } else if (pos<mod*7/8) {
      time_marker = 0b1111111000000;
    } else {
      time_marker = 0b11111111000000;
    } 
    if (loop1State==play) {
      //disp.writeDigitRaw(3, 0b10010000000110);
      disp[0].writeDigitRaw(3, time_marker);
    } else if (loop1State==overdub) {
      disp[0].writeDigitRaw(3, 0b10110100110110);
    } else {
      disp[0].writeDigitRaw(3, 0b0);
    }
    if (loop2State==play) {
      disp[0].writeDigitRaw(2, time_marker);
    } else if (loop2State==overdub) {
      disp[0].writeDigitRaw(2, 0b10110100110110);
    } else {
      disp[0].writeDigitRaw(2, 0b0);
    }
    if (loop3State==play) {
      disp[0].writeDigitRaw(1, time_marker);
    } else if (loop3State==overdub) {
      disp[0].writeDigitRaw(1, 0b10110100110110);
    } else {
      disp[0].writeDigitRaw(1, 0b0);
    }
    if (loop4State==play) {
      disp[0].writeDigitRaw(0, time_marker);
    } else if (loop4State==overdub) {
      disp[0].writeDigitRaw(0, 0b10110100110110);
    } else {
      disp[0].writeDigitRaw(0, 0b0);
    }
  }

  disp[0].writeDisplay();

  delay(100);
}
/*
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
*/
// Resets the buffer
void ResetBuffer() {
  pos = 0;
  len = 0;
  for (int i = 0; i < TOTAL_MAX_SIZE; i++) {
    buf[i] = 0;
  }
  mod = SINGLE_MAX_SIZE;
}

// Deals with analog controls
void Controls() {
  petal.ProcessAllControls();
  UpdateButtons();
}

void UpdateButtons() {
  if (petal.buttons[1].RisingEdge()) { // loop 1 rec
    switch (loop1State) {
      case idle_empty:
        loop1State = armed;
        break;
      case armed:
        loop1State = idle_empty;
        break;
      case rec_first:
        mod = len;
        len = 0;
        loop1State = overdub;
        break;
      case play:
      case idle:
        loop1State = overdub;
        break;
      case overdub:
        loop1State = play;
        break;
    }
    if (loop2State == overdub) loop2State = play; // downgrade any recording loops to playing
    if (loop3State == overdub) loop3State = play;
    if (loop4State == overdub) loop4State = play;
  }
  if (petal.buttons[3].RisingEdge()) { // loop 2 rec
    switch (loop2State) {
      case idle_empty:
        if (loop1State==idle_empty || loop1State==armed) break;
      case idle:
      case play:
        loop2State = overdub;
        break;
      case (overdub):
        loop2State = play;
        break;
    }
    if (loop1State == rec_first) {
      mod = len;
      len = 0;
      loop1State = play;
    }
    if (loop1State == overdub) loop1State = play; // downgrade any recording loops to playing
    if (loop3State == overdub) loop3State = play;
    if (loop4State == overdub) loop4State = play;
  }
  if (petal.buttons[5].RisingEdge()) { // loop 3 rec
    switch (loop3State) {
      case idle_empty:
        if (loop1State==idle_empty || loop1State==armed) break;
      case idle:
      case play:
        loop3State = overdub;
        break;
      case overdub:
        loop3State = play;
        break;
    }
    if (loop1State == rec_first) {
      mod = len;
      len = 0;
      loop1State = play;
    }
    if (loop1State == overdub) loop1State = play; // downgrade any recording loops to playing
    if (loop2State == overdub) loop2State = play;
    if (loop4State == overdub) loop4State = play;
  }
  if (petal.buttons[7].RisingEdge()) { // loop 4 rec
    switch (loop4State) {
      case idle_empty:
        if (loop1State==idle_empty || loop1State==armed) break;
      case idle:
      case play:
        loop4State = overdub;
        break;
      case overdub:
        loop4State = play;
        break;
    }
    if (loop1State == rec_first) {
      mod = len;
      len = 0;
      loop1State = play;
    }
    if (loop1State == overdub) loop1State = play; // downgrade any recording loops to playing
    if (loop2State == overdub) loop2State = play;
    if (loop3State == overdub) loop3State = play;
  }
  if (petal.buttons[0].RisingEdge()) { // pressed loop 1 play button
    switch (loop1State) {
      case rec_first:
        mod = len;
        len = 0;
        loop1State = play;
        break;
      case overdub:
      case idle:
        loop1State = play;
        break;
      case play:
        loop1State = idle;
        break;
    }
  }
  if (petal.buttons[2].RisingEdge()) { // pressed loop 2 play button
    switch (loop2State) {
      case overdub:
      case idle:
        loop2State = play;
        break;
      case play:
        loop2State = idle;
        break;
    }
  }
  if (petal.buttons[4].RisingEdge()) { // pressed loop 3 play button
    switch (loop3State) {
      case overdub:
      case idle:
        loop3State = play;
        break;
      case play:
        loop3State = idle;
        break;
    }
  }
  if (petal.buttons[6].RisingEdge()) { // pressed loop 4 play button
    switch (loop4State) {
      case overdub:
      case idle:
        loop4State = play;
        break;
      case play:
        loop4State = idle;
        break;
    }
  }
  if (petal.buttons[0].TimeHeldMs() >= 1000 && loop1State != idle_empty) { // hold play button
    ResetBuffer();
    loop1State = idle_empty;
    loop2State = idle_empty;
    loop3State = idle_empty;
    loop4State = idle_empty;
  }
  if (petal.buttons[0].Pressed() && petal.buttons[1].Pressed()) {
    for (int i = 0; i < LOOP2OFFSET; i++) buf[i] = 0;
    loop1State = idle;
  }
  if (petal.buttons[2].Pressed() && petal.buttons[3].Pressed()) {
    for (int i = LOOP2OFFSET; i < LOOP3OFFSET; i++) buf[i] = 0;
    loop2State = idle_empty;
  }
  if (petal.buttons[4].Pressed() && petal.buttons[5].Pressed()) {
    for (int i = LOOP3OFFSET; i < LOOP4OFFSET; i++) buf[i] = 0;
    loop3State = idle_empty;
  }  
  if (petal.buttons[6].Pressed() && petal.buttons[7].Pressed()) {
    for (int i = LOOP4OFFSET; i < TOTAL_MAX_SIZE; i++) buf[i] = 0;
    loop4State = idle_empty;
  }  
  if ((loop1State == idle || loop1State == idle_empty) && 
      (loop2State == idle || loop2State == idle_empty) && 
      (loop3State == idle || loop3State == idle_empty) && 
      (loop4State == idle || loop4State == idle_empty)) pos = 0; // if nothing else is playing, move playback head to start
}

void NextSamples(float &output, float *in, size_t i) {
  // Mirror input to output
  output = in[i];
  // add each playback to output before recording new layers
  if (loop1State!=idle) output += buf[pos];
  if (loop2State!=idle) output += buf[LOOP2OFFSET + pos];
  if (loop3State!=idle) output += buf[LOOP3OFFSET + pos];
  if (loop4State!=idle) output += buf[LOOP4OFFSET + pos];
  // record new layers if recording active for that looper
  if ((loop1State == rec_first) || (loop1State == overdub)) buf[pos] += in[i];
  if (loop2State == overdub) buf[LOOP2OFFSET + pos] += in[i];
  if (loop3State == overdub) buf[LOOP3OFFSET + pos] += in[i];
  if (loop4State == overdub) buf[LOOP4OFFSET + pos] += in[i];
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
}
