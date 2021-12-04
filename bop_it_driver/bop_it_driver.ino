#include "FastLED.h"
#include <LiquidCrystal_I2C.h>

#define NUM_LEDS_PER_STRIP 20
#define COLOR_ORDER RGB

#define ROTARY_A 32
#define ROTARY_B 25

#define DEBUG false

const uint8_t DIGITAL_SWITCH_PIN = 33;       // SET SW PIN

//This is the number of possible events + 1 to account for flooring
#define NUM_EVENTS 3

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;


LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  


// LEDS
CRGB leds[NUM_LEDS_PER_STRIP];
int score = 0;
int count = 0;

// GPIO definitions
int BUTTON_BUTTON = 13;
int WIRE_BUTTON = 12;
int LED_PIN = 27;
int TONE_PIN = 14;

// Custom types for events
enum event_type {
  wire,
  button,
  dial,
  NO_EVENT
};

struct Event {
  event_type event;
  long unsigned int timer_ms;
};

Event* currentEvent;
Event* userEvent;

//global vars
static bool playing = true;     ///CHANGE THIS
event_type noEvent = (event_type) (3);

bool buttonLast = false;
bool wireLast = false;
int rotaryLast = 0;

bool buttonNow = false;
bool wireNow = false;
int rotaryNow = 0;

int delta = 5000;

bool rotaryLatch = false;

//Function Prototypes
void start_menu();


void play_tone(int tone_type, int d){
    ledcAttachPin(TONE_PIN,0);

    //Success
    if(tone_type == 0){
      ledcWriteNote(0, NOTE_C, 4);
      delay(d);
      ledcWriteNote(0, NOTE_C, 4);
      delay(d);
    }

    if(tone_type == 1){
      ledcWriteNote(0, NOTE_C, 4);
      delay(d);
      ledcWriteNote(0, NOTE_C, 5);
      delay(d);
    }

    if(tone_type == 2){
      ledcWriteNote(0, NOTE_C, 4);
      delay(d);
      ledcWriteNote(0, NOTE_C, 3);
      delay(d);
    }

    if(tone_type == 3){
      ledcWriteNote(0, NOTE_Eb, 3);
      delay(d);
      ledcWriteNote(0, NOTE_D, 3);
      delay(d);
      ledcWriteNote(0, NOTE_Cs, 3);
      delay(d);
      ledcWriteNote(0, NOTE_C, 3);
      delay(2*d);

      delay(1000);

      for(int i = 0; i < score/10; i++){
          ledcWriteNote(0, NOTE_F, 3);
          delay(d*2);
          ledcDetachPin(TONE_PIN);
          delay(100);
          ledcAttachPin(TONE_PIN,0);
        }
     

      for(int i = 0; i < score%10; i++){
          ledcWriteNote(0, NOTE_F, 4);
          delay(d);
          ledcDetachPin(TONE_PIN);
          delay(100);
          ledcAttachPin(TONE_PIN,0);
      }     
    }
    
    ledcDetachPin(TONE_PIN);
}


void setup() {
  delay(3000);
  // put your setup code here, to run once:
  FastLED.addLeds<WS2811, 27, COLOR_ORDER>(leds, 20);


  Serial.begin(115200);
  Serial.println("Hello");

  FastLED.clear();

  pinMode(BUTTON_BUTTON, INPUT);
  pinMode(WIRE_BUTTON, INPUT);

  pinMode(ROTARY_A, INPUT);
  pinMode(ROTARY_B, INPUT);

  leds[0].r = 255;
  FastLED.show();


  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();
  
  currentEvent = new Event{noEvent, millis()};
  userEvent = new Event{noEvent, millis()};

  ledcSetup(0, 1E5, 12);

  play_tone(0, 500);

  start_menu();
}


void start_menu(){
  FastLED.clear(true);
  bool prompted = false;

  while(digitalRead(BUTTON_BUTTON) == HIGH){
    score = 0;
    if(!prompted){
      lcd.clear();
      lcd.print("Release the");
      lcd.setCursor(0,1);
      lcd.print("button!");
      prompted = true;
    }
  }
  delay(500);

  lcd.clear();
  
  lcd.setCursor(0,0);

  lcd.print("Do any action");

  lcd.setCursor(0,1);

  lcd.print("to start!");

  debounce_inputs(100, true, true, true);

  while(buttonLast == buttonNow && wireLast == wireNow && rotaryNow == rotaryLast){
    buttonNow = digitalRead(BUTTON_BUTTON);
    wireNow = digitalRead(WIRE_BUTTON);
    rotaryNow = digitalRead(ROTARY_A);
  }

  debounce_inputs(500, true, true, true);

  playing = true;

  currentEvent = new Event{noEvent, millis()};
  userEvent = new Event{noEvent, millis()};
}


void debounce_inputs(int mills, bool but, bool wire, bool rot){
  delay(mills);
  if(but) {
    buttonNow = digitalRead(BUTTON_BUTTON);
    buttonLast = buttonNow;
    }
  if(wire){  
    wireNow = digitalRead(WIRE_BUTTON);
    wireLast = wireNow;
  }
  if(rot){
    rotaryNow = digitalRead(ROTARY_A);
    rotaryLast = rotaryNow;
  }
}


// ------------- GENERATE RANDOM EVENT ----------------
void generate_random_event(){
  int randi = random(NUM_EVENTS);

  //Handling same event cases
  while ((int) (currentEvent -> event) == randi){
    Serial.print("Loop \nevent: ");
    Serial.print((int)(currentEvent -> event));
    Serial.print("\t");
    Serial.print("Rand: ");
    Serial.println(randi);
    randi = random(NUM_EVENTS);
    Serial.print("After randomization: ");
    Serial.println(randi);
  }

  // CHANGE THIS
  event_type e = (event_type) (floor(randi));



//  delete currentEvent;
  currentEvent = new Event {e, millis()};
  userEvent = new Event{noEvent, millis()};

  play_tone((int) currentEvent -> event, 200);


  Serial.println("Generating Random Event");

  lcd.clear();
  lcd.setCursor(0,0);
  if((int) (currentEvent -> event) == 1){
    lcd.print("Push it!");
  }
  else if((int) (currentEvent -> event) == 2){
    lcd.print("Turn it!");
  }
  else if((int) (currentEvent -> event) == 0){
    lcd.print("Plug it!");
  }

  rotaryLatch = false;
}



void game_over(){
  delta = 5000;
  play_tone(3, 300);
  int count = 0;

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Game Over!");
  while(true){
    if(digitalRead(BUTTON_BUTTON) == HIGH){
      Serial.println(count);
      count ++;
    }
    else{
      count = 0;
    }

    if(count >= 2){
      start_menu();
      return;
    }
    Serial.println("Game Over!");
    delay(1000);
  }
}



void checkInput(){

    if(DEBUG){return;}

    if(millis() > currentEvent -> timer_ms + delta){
      playing = false;
      game_over();
    }
    
    //No input made yet
    if(userEvent -> event == noEvent){
      return;
    }

    if(userEvent -> event == currentEvent -> event){
      delta = delta-30;
      
      score++;

      if(score > 10){
        FastLED.clear(true);

        for(int i = 0; i < score/10; i++){
          leds[19-i].r = 255;
        }
      }

      for(int i = 0; i < score%10; i++){
        leds[i].r = 255;
      }      

      FastLED.show();
      
      Serial.println("User press = current event");
      Serial.println(userEvent -> event);
      Serial.println(currentEvent -> event);
      generate_random_event();
    }

    if(userEvent -> event != currentEvent -> event && userEvent -> event != noEvent){
      Serial.println("WRONG INPUT LOSER");
      Serial.println(userEvent -> event);
      playing = false;
      game_over();
    }


  }


void loop() {

  delay (50);
  if(DEBUG){
      rotaryNow = digitalRead(ROTARY_A);

      if(rotaryNow != rotaryLast && rotaryNow == 1){
        count++;
        Serial.println(count);
    }
  }

  if(playing){

      if(currentEvent -> event == noEvent){
        Serial.println("Generating First Event");
        generate_random_event();
      }
      
      delay(10);    
      //Read inputs
      buttonNow = digitalRead(BUTTON_BUTTON);
      wireNow = digitalRead(WIRE_BUTTON);
      rotaryNow = digitalRead(ROTARY_A);
    
      // ------ Polling for input changes ------
      //Button input has changed!
      if(buttonLast != buttonNow){
          if(buttonNow == HIGH){
              Serial.println("Button Pressed");
    
              event_type d = (event_type) 1;
              userEvent -> event = d;
          }

          debounce_inputs(200, true, false, false);
      }

      if(wireLast != wireNow){
          debounce_inputs(200, false, true, false);

          Serial.println("Wire Plugged");

          event_type d = (event_type) 0;
          userEvent -> event = d;
      }

      if(rotaryNow != rotaryLast && rotaryLatch == false){
        rotaryLatch = true;
        debounce_inputs(500, false, false, true);

        event_type d = (event_type) 2;
        userEvent -> event = d;
      }
    
      checkInput();
    
//      Serial.println(String(currentEvent -> event));
//      Serial.println(String(currentEvent -> timer_ms));
  }

}
