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

#define LED_PIN 4
#define NUM_LEDS_PER_BEACON 24
#define NUM_BEACONS 6
#define NUM_LEDS (NUM_LEDS_PER_BEACON * NUM_BEACONS)
#define CHIPSET     WS2811
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];

PatternController pattern_master = PatternController(leds, NUM_LEDS);

void setup(){
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  pattern_master.add(new Usa());
  pattern_master.add(new ColorBars(CRGB::Blue, 4));
  pattern_master.add(new Gradient());
  pattern_master.add(new LarsonScanner());
  pattern_master.add(new Rainbow(8));
  pattern_master.add(new RandomColors(3));
  pattern_master.add(new Sos());
}

void loop(){
  pattern_master.show();
}
