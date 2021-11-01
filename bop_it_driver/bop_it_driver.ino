#include "FastLED.h"
#define NUM_LEDS_PER_STRIP 60

CRGB leds[50];

void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<NEOPIXEL, 22>(leds, NUM_LEDS_PER_STRIP);

  
}

void loop() {
  // put your main code here, to run repeatedly:

  leds[0] = CRGB::White;
  FastLED.show();
  delay(1000);
  leds[1] = CRGB::Red;
  FastLED.show();
  delay(1000);


}
