//#include <Wire.h>
// 
//void setup() {
//  Wire.begin();
//  Serial.begin(115200);
//  Serial.println("\nI2C Scanner");
//}
// 
//void loop() {
//  byte error, address;
//  int nDevices;
//  Serial.println("Scanning...");
//  nDevices = 0;
//  for(address = 1; address < 127; address++ ) {
//    Wire.beginTransmission(address);
//    error = Wire.endTransmission();
//    if (error == 0) {
//      Serial.print("I2C device found at address 0x");
//      if (address<16) {
//        Serial.print("0");
//      }
//      Serial.println(address,HEX);
//      nDevices++;
//    }
//    else if (error==4) {
//      Serial.print("Unknow error at address 0x");
//      if (address<16) {
//        Serial.print("0");
//      }
//      Serial.println(address,HEX);
//    }    
//  }
//  if (nDevices == 0) {
//    Serial.println("No I2C devices found\n");
//  }
//  else {
//    Serial.println("done\n");
//  }
//  delay(5000);          
//}

#include <LiquidCrystal_I2C.h>

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  

void setup(){
  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();
}

void loop(){
  // set cursor to first column, first row
  lcd.setCursor(0, 0);
  // print message
  lcd.print("Hello, Borld!");
  delay(1000);
  // clears the display to print new message
  lcd.clear();
  // set cursor to first column, second row
  lcd.setCursor(3,1);
  lcd.print("Hello, Borld!");
  delay(1000);
  lcd.clear(); 
}
