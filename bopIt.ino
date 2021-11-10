#define BUTTON_IN 18
#define ROTARY_A 32
#define ROTARY_B 33

//TODO
//Fix turnIt so it's not looped
//make check for wrong inputs

int action, point, score = 0;
bool taskComplete = true;

void setup() {
  pinMode(BUTTON_IN, INPUT);
  pinMode(ROTARY_A,INPUT);
  pinMode(ROTARY_B,INPUT);

  //pinALast = digitalRead(ROTARY_A);
  Serial.begin(9600);

  Serial.println("Start!");
  delay(500);
}

void loop() {
  if(taskComplete)//upon startup, taskComplete = true
  {
    point = 0;

    Serial.print("Score: ");
    Serial.println(score);

    //only does pressIt
    newAction();

    taskComplete = false;
    
    switch (action)
    {
      case 0: //again, just doing case 0 rn (see newAction funct)
        Serial.println("Press It!");
        pressIt();
        break;
      case 1:
        Serial.println("Turn It!");
        //turnIt();
        break;
    }
    taskComplete = checkAction();
  }
  else //the "waiting for correct input" zone
  {
    switch (action)
    {
      case 0:
        //Serial.println("Press It!");
        point = pressIt();
        break;
      case 1:
        //Serial.println("Turn It!");
        //point = turnIt();
        break;
    }
    taskComplete = checkAction();
    if (taskComplete)//will only increment score whe taskComplete is rising-edge
      score++;
  }
  
}

int pressIt()
{
  if (digitalRead(BUTTON_IN) == HIGH)
  {
    delay(500);// for douncing-ish reasons
    Serial.println("BUTTON PRESS!");
    return 1;
  }

  return 0;
}

//DUBIOUS CODE INBOUND
//documentation for Rotary Encoder:
//https://www.handsontec.com/dataspecs/module/Rotary%20Encoder.pdf
int turnIt()
{
  static int aVal, pinALast, turnPercent = 0;
//  
//  do {
//      
//      aVal = digitalRead(ROTARY_A);
//          
//      if (aVal != pinALast){
//
//        Serial.println("pinALast");
//        Serial.println(pinALast);
//        
//        Serial.println("aVal");
//        Serial.println(aVal);
//        
//          turnPercent += 5;
//         if (turnPercent > 100)
//           turnPercent -= 100;
//        
//         //cute little loading animation
//         Serial.print(turnPercent);
//         Serial.println("%");
//         Serial.print("0%\t[");
//         for (int i = 0; i < turnPercent/5; i++)
//           Serial.print("=");
//         for (int i = 0; i < (20-turnPercent/5); i++)
//           Serial.print(" ");
//         Serial.println("]\t\t100%");
//    }
//    
//    pinALast = aVal;
//    } while (turnPercent != 100);
//    
//  delay(500);
  return 1;
}

//TODO implement this for score == 100, reset taskComplete
void reset()
{
  score = 0;
  point = 0;
}

void newAction()//again, again, not including turnIt
{
  action = random(0,1); // [0,2) not including 2
}

bool checkAction()//the "blank"It funct will return a 1 on successful check
{
  if (point == 1)
    return true;
  return false;
}
