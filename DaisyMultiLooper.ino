#include "DaisyDuino.h"
#include "utility/gpio.h" // needed for reset to bootloader

#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
#define DISPLAY_ADDRESS 112

Adafruit_AlphaNum4 disp[8];

static DaisyHardware petal;

unsigned long lastStartTime;
unsigned long startTime;
unsigned long stopTime;
int timeCalcLoop=0;

enum UIStateType {
  UI_Welcome,
  UI_Anim,
  UI_FX, // shows all different FX
  UI_Dist, // shows distortion sub-menu
  UI_Loop, // shows loop status and play/rec buttons
  UI_Loop2, // shows extra loop features, like kill all, join, tape stop etc.
  UI_Echo, // used in all loops
  UI_BOOT // used when put into bootloader mode
};
UIStateType UI = UI_Welcome;

void setup() {
  for (int i=0; i<8; i++) {
    disp[i] = Adafruit_AlphaNum4();
    disp[i].begin(DISPLAY_ADDRESS+i);
    disp[i].setBrightness(4);
    disp[i].clear();
    disp[i].writeDisplay();
  }
  petal = DAISY.init(DAISY_PETAL, AUDIO_SR_48K);
  DAISY.SetAudioBlockSize(48); // default is 48 samples for 1ms blocksize

  //------------------------------------------------------------------------------------------------------------
  DAISY.begin(dist_AudioCallback);
  //------------------------------------------------------------------------------------------------------------

  Serial.begin(115200);

  //------------------------------------------------------------------------------------------------------------
  dist_setup();
  //------------------------------------------------------------------------------------------------------------
}


void resetToBootloader() {
  UI = UI_BOOT;

  delay(1000);
  // Initialize Boot Pin
  dsy_gpio_pin bootpin = {DSY_GPIOG, 3};
  dsy_gpio pin;
  pin.mode = DSY_GPIO_MODE_OUTPUT_PP;
  pin.pin = bootpin;
  dsy_gpio_init(&pin);
  dsy_gpio_write(&pin, 1);
  delay(10);
  HAL_NVIC_SystemReset();
}

void loop() {
  lastStartTime = startTime;
  startTime = micros();
  petal.ProcessAllControls();
  UI_Menu_Buttons();
  //------------------------------------------------------------------------------------------------------------
  //dist_loop();
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
  //------------------------------------------------------------------------------------------------------------
  stopTime = micros();
  timeCalcLoop++;
  if (timeCalcLoop>10) {
    /*
    Serial.print("Interval: ");
    Serial.println(startTime-lastStartTime);
    Serial.print("Execution time: ");
    Serial.println(stopTime-startTime);
    */
    timeCalcLoop=0;
  }
  UI_Display();
}

void display(uint8_t n, uint16_t a, uint16_t b, uint16_t c, uint16_t d) {
  disp[n].writeDigitRaw(0, a);
  disp[n].writeDigitRaw(1, b);
  disp[n].writeDigitRaw(2, c);
  disp[n].writeDigitRaw(3, d);
  disp[n].writeDisplay();
}