
void setup() {
  // put your setup code here, to run once:

  // 1-7    PD0-PD7
  // 8-X    PB0-PBX
  //This is the bottom left digital output pin
  pinMode(8, INPUT);
  digitalWrite(8, LOW);
  pinMode(7, OUTPUT);
 

}

// This is a new comment for Adam Johnson!

void loop() {
  // Poll the button press
  if(digitalRead(8) == HIGH){
    digitalWrite(7, HIGH);
    delay(1000);
    digitalWrite(7, LOW);
    delay(1000);
  }
  else{
    digitalWrite(7,LOW);
  }

}
