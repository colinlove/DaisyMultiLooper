  float SoftGain = 2;
  float HardGain = 2;
  float OutGain = 0.25;
  int digitInc=0;

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

static void dist_setup() {
// setup
}

static void dist_AudioCallback(float **in, float **out, size_t size) {
  for (size_t i = 0; i < size; i++) {
    for (int chn = 0; chn < 2; chn++) {
      float signal = in[chn][i];
      signal *= SoftGain;
      signal = softClip(signal);
      signal *= HardGain;
      signal = hardClip(signal);
      signal *= OutGain;
      out[chn][i] = signal;
    }
  }
}


void dist_loop() {
  if (petal.buttons[0].RisingEdge()) {SoftGain*=0.5;Serial.print("SoftGain: ");Serial.println(SoftGain);}
  if (petal.buttons[1].RisingEdge()) {SoftGain*=2;Serial.print("SoftGain: ");Serial.println(SoftGain);}
  if (petal.buttons[2].RisingEdge()) {HardGain*=0.5;Serial.print("HardGain: ");Serial.println(HardGain);}
  if (petal.buttons[3].RisingEdge()) {HardGain*=2;Serial.print("HardGain: ");Serial.println(HardGain);}
  if (petal.buttons[4].RisingEdge()) {OutGain*=0.5;Serial.print("OutGain: ");Serial.println(OutGain);}
  if (petal.buttons[5].RisingEdge()) {OutGain*=2;Serial.print("OutGain: ");Serial.println(OutGain);}


  delay(1);
}