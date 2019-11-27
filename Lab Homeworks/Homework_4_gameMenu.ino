#include <LiquidCrystal.h>

const int pinSW = 10;
const int pinX = A0;
const int pinY = A1;
int xValue = 0;
int yValue = 0;

bool joyMovedY = false;
bool joyMovedX = false;
int minThreshold = 400;
int maxThreshold = 600;

int swState = LOW;
int lastSwState = LOW;
int pressed = 0;
int option = 0;

const int RS = 12;
const int enable = 11;
const int d4 = 5;
const int d5 = 4;
const int d6 = 3;
const int d7 = 2;
LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);

int startingLevelValue = 0;
int score = 0;
int highScore = 0;

unsigned long Time;
unsigned long currTime;

bool change = true;
bool start = false;

bool message1 = 0;
bool message2 = 0;
bool message3 = 0;

bool growLevelFirst = 0;
bool growLevelSecond = 0;

void Option() {

  xValue = analogRead(pinX);
  
  if (xValue > maxThreshold && joyMovedX == false) {
    if(option < 2)
     option ++;
    else option = 0;

    joyMovedX = true;
  }
  
  if (xValue < minThreshold && joyMovedX == false) {
    if(option > 0)
     option --;
    else option = 2;

    joyMovedX = true;
  }

  if(xValue <= maxThreshold && xValue >= minThreshold) {
    joyMovedX = false;
  }
   

   lcd.setCursor(0, 0);
   lcd.print(" ");
   lcd.setCursor(9, 1);
   lcd.print(" ");
   lcd.setCursor(0, 1);
   lcd.print(" ");

  if(option == 0) {
   lcd.setCursor(0, 0);
   lcd.print(">");
  }
  
  if(option == 1) {
   lcd.setCursor(0, 1);
   lcd.print(">");
  }

  if(option == 2) {
   lcd.setCursor(9, 1);
   lcd.print(">");
  }
  
}

void Menu() {
    
  lcd.setCursor(1, 0);
  lcd.print("StartGame");

  lcd.setCursor(1, 1);
  lcd.print("Settings");

  lcd.setCursor(10, 1);
  lcd.print("HScore");

  Option();
}

int Select() {
  swState = digitalRead(pinSW);
  if(swState != lastSwState) {
   change = true;
   if(swState == LOW) {
     pressed = !pressed;
   }
  }
 lastSwState = swState;
 
 return pressed;
}


void Jump() {
  if(Select() == 0) {
    if(change == true) {
     lcd.clear();
     change = false; 
    }
    Menu();
  }
  else {
    if(option == 0) {
     lcd.clear();
     
     currTime = millis();
     Time = currTime;
     growLevelFirst = 0;
     growLevelSecond = 0;
     message1 = 0;
     message2 = 0;
     message3 = 0;

      while(Select() == 1) {
       if(millis() - Time < 11000) {
       lcd.setCursor(0, 0);
       lcd.print("Lives:3");
       lcd.setCursor(8, 0);
       lcd.print("Level:");
       lcd.print(startingLevelValue);
       lcd.setCursor(0, 1);
       lcd.print("Score:");
       score = startingLevelValue * 3;
       if(score > highScore)
        highScore = score;
       lcd.print(score);
       }
       if(millis() - Time >= 5000) {
        if(millis()- Time < 10000 && growLevelFirst == 0) {
         startingLevelValue++;
         growLevelFirst = 1;
        }
       }
       if(millis() - Time >= 10000 && millis() - Time < 11000) {
        if(growLevelSecond == 0) {
         startingLevelValue++;
         growLevelSecond = 1;
        }
       }
      
       if(millis() - Time >= 11000 && millis() - Time < 12000){
        if(message1 == 0) {
          lcd.clear();
          message1 = 1;
        }
        lcd.setCursor(0, 0);
        lcd.print("Congratulations!");

        startingLevelValue = 0;
       }
       if(millis() - Time >= 12000 && millis() - Time < 13000) {
        if(message2 == 0) {
          lcd.clear();
          message2 = 1;
        }
        lcd.setCursor(0, 0);
        lcd.print("You finished");
        lcd.setCursor(3, 1);
        lcd.print("the game");
       }
       if(millis() - Time >= 13000) {
        if(message3 == 0) {
         lcd.clear();
         message3 = 1;
        }
        lcd.setCursor(0, 0);
        lcd.print("Press button");
        lcd.setCursor(3, 1);
        lcd.print("to exit");
       }
        
      }
     }
    
    if(option == 1) {
      lcd.clear();
      startingLevelValue = 0;
      
      while(Select() == 1) {
      yValue = analogRead(pinY);
  
     if (yValue > maxThreshold && joyMovedY == false) {
      startingLevelValue++;
      joyMovedY = true;
     }
     
  
     if (yValue < minThreshold && joyMovedY == false) {
      if(startingLevelValue > 0)
       startingLevelValue--;
      joyMovedY= true;
     }

     if(yValue <= maxThreshold && yValue >= minThreshold) {
      joyMovedY = false;
     }

     score = startingLevelValue * 3;
     if(score > highScore)
      highScore = score; 
     
     lcd.setCursor(0, 0);
     lcd.print("Select level");
     lcd.setCursor(0, 1);
     lcd.print(startingLevelValue);
   }
  }
   
   if(option == 2) {
    lcd.clear();

    while(Select() == 1) {
     lcd.setCursor(0, 0);
     lcd.print("High Score");
     lcd.setCursor(0, 1);
     lcd.print(highScore);
    }
   }
  }
}


void setup() {
  // put your setup code here, to run once:
  pinMode(pinX, INPUT);
  pinMode(pinY, INPUT);
  pinMode(pinSW, INPUT_PULLUP);

  lcd.begin(16, 2);
  
  Serial.begin(9600);
} 

void loop() {
  // put your main code here, to run repeatedly:
  Jump();
}
