#include "Arduino.h"
#include "PatternController.h"
#include "patterns/ColorBars.h"
#include "patterns/Generic.h"
#include "patterns/Gradient.h"
#include "patterns/LarsonScanner.h"
#include "patterns/Rainbow.h"
#include "patterns/RandomColors.h"
#include "patterns/Sos.h"
#include "patterns/Usa.h"
#include <FastLED.h>

#define DEBUG

#define POWER_LED_PIN A3

#define LED_PIN 4
#define NUM_LEDS_PER_BEACON 24
#define NUM_BEACONS 6
#define NUM_LEDS (NUM_LEDS_PER_BEACON * NUM_BEACONS)
#define CHIPSET     WS2811
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];

PatternController pattern_master = PatternController(leds, NUM_LEDS);

void setup(){
  #ifdef DEBUG
    Serial.begin(115200);
  #endif
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness(50);
  pattern_master.add(new Usa(8));
  pattern_master.add(new ColorBars(CRGB::Blue, 4));
  pattern_master.add(new LarsonScanner(CRGB::Green, CRGB::Orange));
  pattern_master.add(new RandomColors(3));
  pattern_master.add(new Rainbow(8));
  pattern_master.add(new Sos());
  pattern_master.add(new Gradient());
  pattern_master.set_fps(30);
  pattern_master.pattern_duration = 10000;
  //Status pin so I know that we made it through Setup
  pinMode(POWER_LED_PIN, OUTPUT);
  digitalWrite(POWER_LED_PIN, HIGH);
}

void loop(){
  pattern_master.show();
  FastLED.show();
}
