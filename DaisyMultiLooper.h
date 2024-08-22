
enum ChannelState {
  idle_empty,
  armed,
  rec_first,
  play, // used in all loops
  overdub, // used in all loops
  idle // used in all loops
};

//static void AudioCallback(float **in, float **out, size_t size);

void display(uint8_t n, uint16_t a, uint16_t b, uint16_t c, uint16_t d);

void displayloopstatus(uint8_t d1, uint8_t d2, ChannelState loopstate);

void UI_Display();

void UI_Menu_Buttons();

void NextSamples(float &output, float *in, size_t i);

void ResetBuffer();