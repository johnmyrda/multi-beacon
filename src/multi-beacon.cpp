#include "Arduino.h"
#include "PatternController.h"
#include "patterns/ColorBars.h"
#include "patterns/Generic.h"
#include "patterns/Gradient.h"
#include "patterns/LarsonScanner.h"
#include "patterns/BeaconPulse.h"
#include "patterns/BeaconPulse2.h"
#include "patterns/BeaconScanner.h"
#include "patterns/Rainbow.h"
#include "patterns/RandomColors.h"
#include "patterns/Sos.h"
#include "patterns/Usa.h"
#include "patterns/Lightning.h"
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

  // Pattern * usa = new Usa(24);
  // usa->set_fps(45);
  // pattern_master.add(usa);
  //
  // Pattern * colorbars = new ColorBars(CRGB::Blue, CRGB::Black, 24);
  // pattern_master.add(colorbars);
  //
  // Pattern * larson = new LarsonScanner(CRGB::Red, CRGB::Black);
  // larson->set_fps(80);
  // pattern_master.add(larson);

  // Pattern * randomcolors = new RandomColors(4);
  // randomcolors->set_fps(15);
  // pattern_master.add(randomcolors);
  pattern_master.add(new BeaconPulse2());
  pattern_master.add(new Rainbow(1));
  //pattern_master.add(new Sos());
  pattern_master.add(new Gradient(CRGB::Green));

  Pattern * bp = new BeaconPulse();
  bp->set_fps(20);
  pattern_master.add(bp);

  pattern_master.add(new BeaconScanner());
  pattern_master.add(new Lightning(CRGB::White, 24));
  pattern_master.set_fps(30);
  pattern_master.pattern_duration = 12000;

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
