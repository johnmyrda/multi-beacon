#include "Arduino.h"
#include "PatternController.h"
#include "patterns/AlternatingColors.h"
#include "patterns/BeaconPulse2.h"
#include "patterns/BeaconPulse.h"
#include "patterns/BeaconScanner.h"
#include "patterns/ColorBars.h"
#include "patterns/Gradient.h"
#include "patterns/LarsonScanner.h"
#include "patterns/Lightning.h"
#include "patterns/Rainbow.h"
#include "patterns/RandomColors.h"
#include <FastLED.h>
#include <TimerOne.h>

#define POWER_LED_PIN A3
#define KNOB_PIN A0
#define SWITCH_PIN 3

#define LED_PIN 4
#define NUM_LEDS_PER_BEACON 24
#define NUM_BEACONS 6
#define NUM_LEDS (NUM_LEDS_PER_BEACON * NUM_BEACONS)
#define CHIPSET     WS2811
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];

PatternController pattern_master = PatternController(leds, NUM_LEDS);

AlternatingColors alt = AlternatingColors(24);
ColorBars cb = ColorBars(CRGB::Blue, CRGB::Black, 24);
LarsonScanner larson = LarsonScanner(CRGB::Red, CRGB::Black);
RandomColors randomcolors = RandomColors(4);
BeaconPulse2 bp2 = BeaconPulse2();
Rainbow rainbow = Rainbow(1);
Gradient grad = Gradient();
BeaconPulse bp = BeaconPulse();
BeaconScanner bs = BeaconScanner();
Lightning lightning = Lightning(CRGB::White, 24);

// These values are used in updateBrightness().
uint8_t brightnesses[] = {7, 20, 35, 60, 80, 120, 255};
uint8_t num_brightnesses = sizeof(brightnesses) / sizeof(brightnesses[0]);

// This function will be called periodically to update the brightness levels.
void updateBrightness() {
  if(digitalRead(SWITCH_PIN) == HIGH){
    FastLED.setBrightness(0);
  } else {
    uint16_t knob_val = analogRead(KNOB_PIN);
    uint8_t index = (num_brightnesses * knob_val) >> 10;
    FastLED.setBrightness(brightnesses[index]);
  }
}

void setup(){
  #ifdef DEBUG
    Serial.begin(115200);
  #endif
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);

  pattern_master.add(&alt);
  pattern_master.add(&cb);

  larson.set_fps(80);
  pattern_master.add(&larson);

  randomcolors.set_fps(15);
  pattern_master.add(&randomcolors);

  pattern_master.add(&bp2);
  //pattern_master.add(new Sos());
  pattern_master.add(&grad);

  bp.set_fps(20);
  pattern_master.add(&bp);
  pattern_master.add(&rainbow);

  pattern_master.add(&bs);
  pattern_master.add(&lightning);
  pattern_master.set_fps(30);
  pattern_master.pattern_duration = 15000;

  // Set up switch which is used to turn leds all the way off
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  // Set up timer to run the brightness button checks periodically
  Timer1.initialize(100000);  // Period in Microseconds = 0.1 seconds
  Timer1.attachInterrupt(updateBrightness);
  updateBrightness();

  //Status pin so I know that we made it through Setup
  pinMode(POWER_LED_PIN, OUTPUT);
  digitalWrite(POWER_LED_PIN, HIGH);
}

void loop(){
  pattern_master.show();
  FastLED.show();
}
