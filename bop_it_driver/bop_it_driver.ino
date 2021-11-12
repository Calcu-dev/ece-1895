#include "FastLED.h"
#define NUM_LEDS_PER_STRIP 50
#define COLOR_ORDER RGB

CRGB leds[50];
int score;

void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<WS2811, 22, COLOR_ORDER>(leds, NUM_LEDS_PER_STRIP);

  pinMode(21, INPUT);

  score = 0;

//  setupLED();
}

void loop() {

  leds[score / 10].r = 0;
  leds[score % 10].r = 0;  

  delay(500);

  if(score > 30)
  {
    score = 0;
  }
  score++;

  leds[score / 10].r = 255;
  leds[score % 10].r = 255;

  FastLED.show();

  if(digitalRead(21) == HIGH){
    printf("HIGH\n");
  }
  else{
    printf("LOW\n");
  }
}

void setupLED(){
//  int k = 255;
//  for(int i = 0; i < NUM_LEDS_PER_STRIP; i++){
//    leds[i].r = k;
//  }
//  FastLED.show();
}
