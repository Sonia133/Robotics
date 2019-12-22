#include <LiquidCrystal.h>
#include <LedControl.h>
#include <EEPROM.h>

LedControl lc = LedControl(8, 10, 9, 1);

const int pinX = A0;
const int pinY = A1;
const int pinSW = 1;

int swState = LOW;
int lastSwState = LOW;

bool joyMovedY = false;
bool joyMovedX = false;

int minThreshold = 400;
int maxThreshold = 600;

int xValue = 0;
int yValue = 0;

int posX = 6;
int posY = 3;

int prevX;
int prevY;

// Select()
int pressed = 0;

const int ledRed = 7;
const int buzzerPin = 6;

// Option() (on lcd)
int option = 0;

const int RS = 12;
const int enable = 11;
const int d4 = 5;
const int d5 = 4;
const int d6 = 3;
const int d7 = 2;
LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);

// shifting matrix according to the current speed
unsigned long currTime = 0;

// Refresh menu in Jump()
bool change = true;

// winningMessage()
bool message1 = 0;
bool message2 = 0;
bool message3 = 0;
bool message4 = 0;

//Info in Settings
bool messageInfo1 = 0;
bool messageInfo2 = 0;
bool messageInfo3 = 0;
bool messageInfo4 = 0;
unsigned long printInfo = 0;

long nextLevel = -14000;

// player chose StartGame
bool player = 0;
bool justStarted = 0;
bool startMess = 1;
bool startScrolling = 0;
int cntObstacle = 0;
bool shift = 1;
int currSpeed = 200;
int startingLevelValue = 1;
int lives = 3;
int score = 0;
int intermScore = 0;
int pressedStart = 4;

bool lost = 0;
unsigned int stopGame = 0;

// fillMatrix() (prevents obstacles to repeat)
int lastObs;

int highScore = 0;

int nmbLevels[] = {
  10, 10, 15, 15, 10
};

bool Matrix[14][8] = {
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0},
{1, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 1}
};

bool one[8][8] = {
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 1, 0, 0, 0},
{0, 0, 0, 0, 1, 0, 0, 0},
{0, 0, 0, 0, 1, 0, 0, 0},
{0, 0, 0, 0, 1, 0, 0, 0},
{0, 0, 0, 0, 1, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0}
};

bool two[8][8] = {
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 1, 1, 1, 0, 0},
{0, 0, 0, 0, 0, 1, 0, 0},
{0, 0, 0, 1, 1, 1, 0, 0},
{0, 0, 0, 1, 0, 0, 0, 0},
{0, 0, 0, 1, 1, 1, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0}
};

bool three[8][8] = {
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 1, 1, 1, 0, 0},
{0, 0, 0, 0, 0, 1, 0, 0},
{0, 0, 0, 1, 1, 1, 0, 0},
{0, 0, 0, 0, 0, 1, 0, 0},
{0, 0, 0, 1, 1, 1, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0}
};

bool four[8][8] = {
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 1, 0, 1, 0, 0},
{0, 0, 0, 1, 0, 1, 0, 0},
{0, 0, 0, 1, 1, 1, 0, 0},
{0, 0, 0, 0, 0, 1, 0, 0},
{0, 0, 0, 0, 0, 1, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0}
};

bool five[8][8] = {
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 1, 1, 1, 0, 0},
{0, 0, 0, 1, 0, 0, 0, 0},
{0, 0, 0, 1, 1, 1, 0, 0},
{0, 0, 0, 0, 0, 1, 0, 0},
{0, 0, 0, 1, 1, 1, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0}
};

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

// choose option in Menu
void Option() {

  xValue = analogRead(pinX);
  
  if (xValue > maxThreshold && joyMovedX == false) {
    if(option < 3)
     option ++;
    else option = 0;

    joyMovedX = true;
  }
  
  if (xValue < minThreshold && joyMovedX == false) {
    if(option > 0)
     option --;
    else option = 3;

    joyMovedX = true;
  }

  if(xValue <= maxThreshold && xValue >= minThreshold) {
    joyMovedX = false;
  }
   

   lcd.setCursor(0, 0);
   lcd.print(" ");
   lcd.setCursor(10, 0);
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
   lcd.setCursor(10, 0);
   lcd.print(">");
  }
  
  if(option == 3) {
    lcd.setCursor(9, 1);
    lcd.print(">");
  }
}

void Menu() {
    
  lcd.setCursor(1, 0);
  lcd.print("StartGame");

  lcd.setCursor(11, 0);
  lcd.print("Info");
  
  lcd.setCursor(1, 1);
  lcd.print("Settings");

  lcd.setCursor(10, 1);
  lcd.print("HScore");

  Option();
}

// returns Joysticks button state
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

void StartGame() {
 lcd.clear();
 
 if(Select() == 1) {
  lcd.setCursor(0, 0);
  lcd.print("Lives:");
  
  lcd.setCursor(6, 0);
  lcd.print(lives);
  
  lcd.setCursor(8, 0);
  lcd.print("Level:");
  lcd.print(startingLevelValue);
  
  lcd.setCursor(0, 1);
  lcd.print("Score:");
  lcd.print(score);
  
  if(score > highScore)
   highScore = score;
  EEPROM.put(0, highScore);
 }
}

// jumps from Menu to the selected option
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
     StartGame();
    }
    
    if(option == 1) {
      lcd.clear();
      startingLevelValue = 1;
      
      while(Select() == 1) {
      yValue = analogRead(pinY);
     
     if (yValue > maxThreshold && joyMovedY == false) {
      if(startingLevelValue < 5)
       startingLevelValue++;
      joyMovedY = true;
     }
     
  
     if (yValue < minThreshold && joyMovedY == false) {
      if(startingLevelValue > 1)
       startingLevelValue--;
      joyMovedY= true;
     }

     if(yValue <= maxThreshold && yValue >= minThreshold) {
      joyMovedY = false;
     }
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
    
    messageInfo1 = 0;
    messageInfo2 = 0;
    messageInfo3 = 0;
    messageInfo4 = 0;
    
    printInfo = millis();

  
    while(Select() == 1) {
     if (millis() - printInfo < 3000) {
      if (messageInfo1 == 0) {
       lcd.clear();
       messageInfo1 = 1;
      }
      lcd.setCursor(0, 0);
      lcd.print("Game name:");
      lcd.setCursor(0, 1);
      lcd.print("BitRace");
     }

     if (millis() - printInfo >= 3000 && millis() - printInfo < 5000) {
      if (messageInfo2 == 0) {
        lcd.clear();
        messageInfo2 = 1;
      }
      lcd.setCursor(0, 0);
      lcd.print("Creater name:");
      lcd.setCursor(0, 1);
      lcd.print("Sorina Andrei");
     }

     if (millis() - printInfo >= 5000 && millis() - printInfo < 7000) {
      if (messageInfo3 == 0) {
        lcd.clear();
        messageInfo3 = 1;
      }
      lcd.setCursor(0, 0);
      lcd.print("Made at");
      lcd.setCursor(0, 1);
      lcd.print("@UniBucRobotics");
     }
     if (millis() - printInfo >= 7000) {
      if(messageInfo4 == 0){
        lcd.clear();
        messageInfo4 = 1;
      }
      lcd.setCursor(1, 0);
      lcd.print("Press button");
      lcd.setCursor(4, 1);
      lcd.print("to exit");
     }
    }
   }
   if(option == 3) {
    lcd.clear();

    while(Select() == 1) {
     lcd.setCursor(0, 0);
     lcd.print("High Score");
     
     lcd.setCursor(0, 1);
     EEPROM.get(0, highScore);
     lcd.print(highScore);
    }
   }
  }
  pressedStart = option;
}

// scrolls your character (red dot on matrix) when the game/level starts
void Scroll() {
 if(startScrolling == 0) {
  posX = 6;
  posY = 3;
  startScrolling = 1;
 }
  
  prevX = posX;
  prevY = posY;
  
  xValue = analogRead(pinX);
  
  if(xValue < minThreshold && joyMovedX == false) {
     if(posY <= 1);
     else posY--;
     
  joyMovedX = true;
  }
    
  if(xValue > maxThreshold && joyMovedX == false) {
   if(posY >= 6);
   else posY++;
   
  joyMovedX = true;
  }
  
  if(xValue >= minThreshold && xValue <= maxThreshold) {
   joyMovedX = false;
  }

  lc.setLed(0, prevX, prevY, false);
  lc.setLed(0, posX, posY, true);
}

//controls the red led when the player lose lives
void lostLives(int life) {
  for(int i = 0; i < life; i++) {
    analogWrite(ledRed, 128);
    delay(100);
    
    analogWrite(ledRed, 0);
    delay(300);
  }
}

void Sing() {
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(buzzerPin, melody[thisNote], noteDuration);
    
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);

    noTone(buzzerPin);
  }
}

// returns the number of the obstacle which will appear next on the matrix
int randMap(){
  int randNumb = random(1, 5);
  return randNumb;
}

// opening message (countdown from 3)
void startMessage() {
   
  for (int row = 0; row < 8; row++) 
   for (int col = 0; col < 8; col++) 
      lc.setLed(0, row, col, three[row][col]);
  delay(1500);
      
  for (int row = 0; row < 8; row++) 
   for (int col = 0; col < 8; col++) 
      lc.setLed(0, row, col, two[row][col]);
  delay(1500);
      
  for (int row = 0; row < 8; row++) 
   for (int col = 0; col < 8; col++) 
      lc.setLed(0, row, col, one[row][col]);
  delay(1500);
}

// checks if another obstacle should be added
int verifyUpperMatrix() {
  int ok = 1;
  for(int row = 0; row < 6; row++)
   for(int col = 0; col < 8; col++)
    if(Matrix[row][col] == 1) 
      ok = 0;
      
  return ok;
}

void fillFirstObstacle() {
  for(int row = 0; row < 4; row++)
   if(row == 0 || row == 3) 
    Matrix[row][1] = 1;
   else {
     Matrix[row][1] = 1;
     Matrix[row][2] = 1;
     Matrix[row][3] = 1;
   }
}

void fillSecondObstacle() {
  for(int row = 0; row < 4; row++)
   if(row == 0 || row == 3) 
    Matrix[row][6] = 1;
   else {
     Matrix[row][6] = 1;
     Matrix[row][5] = 1;
     Matrix[row][4] = 1;
   }
}

void fillThirdObstacle() {
  for(int row = 0; row < 4; row++)
   if(row == 0 || row == 3) {
    Matrix[row][6] = 1;
    Matrix[row][1] = 1;
   }
   else {
     Matrix[row][6] = 1;
     Matrix[row][5] = 1;
     Matrix[row][1] = 1;
     Matrix[row][2] = 1;
   }
}

void fillFourthObstacle() {
  Matrix[1][3] = 1;
  Matrix[1][4] = 1;
  Matrix[2][3] = 1;
  Matrix[2][4] = 1;
  Matrix[3][3] = 1;
  Matrix[3][4] = 1;
}

// add one obstacle
void fillMatrix() {
 int nmbObst = randMap();
 
 while(nmbObst == lastObs)
  nmbObst = randMap();
 lastObs = nmbObst;
 
 if(nmbObst == 1)
  fillFirstObstacle();
 if(nmbObst == 2)
  fillSecondObstacle();
 if(nmbObst == 3)
  fillThirdObstacle();
 if(nmbObst == 4)
  fillFourthObstacle();
}

void shiftMatrix() {
  for(int row = 0; row < 8; row++){
   lc.setLed(0, row, 0, true); 
   lc.setLed(0, row, 7, true);
  }
  for(int row = 13; row >= 0; row--) 
   for(int col = 1; col < 7; col++)
    if(row == 0)
     Matrix[row][col] = 0;
    else
     Matrix[row][col] = Matrix[row - 1][col];
  for(int row = 6; row < 14; row++)
   for(int col = 0; col < 8; col++)
    lc.setLed(0, row - 6, col, Matrix[row][col]);
}

int verifyHit() {
  if(Matrix[posX + 6][posY] == 1)
   return 1;
  return 0;
}

void showLevel() {
   if(startingLevelValue == 1) {
    for (int row = 0; row < 8; row++) 
     for (int col = 0; col < 8; col++) 
      lc.setLed(0, row, col, one[row][col]);
    delay(1500);
    }
    else if(startingLevelValue == 2) {
     for (int row = 0; row < 8; row++) 
      for (int col = 0; col < 8; col++) 
       lc.setLed(0, row, col, two[row][col]);
     delay(1500);
    }
    else if(startingLevelValue == 3) {
     for (int row = 0; row < 8; row++) 
      for (int col = 0; col < 8; col++) 
       lc.setLed(0, row, col, three[row][col]);
     delay(1500);
    }
    else if(startingLevelValue == 4) {
     for (int row = 0; row < 8; row++) 
      for (int col = 0; col < 8; col++) 
       lc.setLed(0, row, col, four[row][col]);
     delay(1500);
    }
    else if(startingLevelValue == 5) {
     for (int row = 0; row < 8; row++) 
      for (int col = 0; col < 8; col++) 
       lc.setLed(0, row, col, five[row][col]);
     delay(1500);
    }
}

void winningMessage() {
   if(startMess == 1) {
    if(message1 == 0) {
     lcd.clear();
     message1 = 1;
    }
   lcd.setCursor(0, 0);
   lcd.print("Congratulations!");

   delay(1500);
    
   if(message2 == 0) {
    lcd.clear();
    message2 = 1;
   }
   lcd.setCursor(3, 0);
   lcd.print("You finished");
   lcd.setCursor(3, 1);
   lcd.print("the game");
    
   delay(1500);
   
   lcd.clear();
   if(score > highScore) {
    lcd.setCursor(3, 0);
    lcd.print("New");
    lcd.setCursor(1, 1);
    lcd.print("HighScore");
    delay(1500);
    lcd.clear();
   }
   
   if(message4 == 0) {
    lcd.clear();
    message4 = 1;
   }
   lcd.setCursor(5, 0);
   lcd.print("Score");
   lcd.setCursor(5, 1);
   lcd.print(score);
  
   delay(2000);
   startMess = 0;
   }
   if(message3 == 0) {
    lcd.clear();
    message3 = 1;
   }
   lcd.setCursor(0, 0);
   lcd.print("Press button");
   lcd.setCursor(3, 1);
   lcd.print("to exit");
   score = 0;
   player = 0;
   pressedStart = 4;
   startScrolling = 0;
   lives = 3;
   currSpeed = 200;
   justStarted = 0;
   cntObstacle = 0;
}

void losingMessage() {
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("You lost");
  delay(1500);
      
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Score");
      
  lcd.setCursor(3, 1);
  lcd.print(score);
  delay(1500);
      
  lcd.clear();
  if(score > highScore) {
   lcd.setCursor(3, 0);
   lcd.print("New");
        
   lcd.setCursor(1, 1);
   lcd.print("HighScore");
   delay(1500);
        
   lcd.clear();
  }
  lcd.setCursor(3, 0);
  lcd.print("Press button");
       
  lcd.setCursor(3, 1);
  lcd.print("to exit");
       
  while(Select() == 1) {
   delay(10);
  }
  lcd.clear();
  score = 0;
  startingLevelValue = 1;
  player = 0;
  pressedStart = 4;
  startScrolling = 0;
  lives = 3;
  currSpeed = 200;
  justStarted = 0;
  lost = 0;
}

void setup() {
  // put your setup code here, to run once:
  lc.shutdown(0, false);
  lc.setIntensity(0, 2);
  lc.clearDisplay(0);
  
  pinMode(pinX, INPUT);
  pinMode(pinY, INPUT);
  pinMode(pinSW, INPUT_PULLUP);
  
  lcd.begin(16, 2);
   
  pinMode(buzzerPin, OUTPUT);

  randomSeed(analogRead(0));
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

 if(startingLevelValue < 6) {
  if((Select() == 0) || (pressedStart != 0)) {
   Jump();
  }
  else { 
   if(player == 0) {
    StartGame();
    player = 1;
    startMessage();
    delay(1000);
    for(int row = 0; row < 8; row++)
     for(int col = 0; col < 8; col++)
      lc.setLed(0, row, col, false);
    delay(1000);
    justStarted = 1;
    if(startingLevelValue != 1)
     currSpeed -= 20*(startingLevelValue - 1);
   }
   if(shift == 1) {
    currTime = millis();
    shift = 0;
   }
   if(cntObstacle < nmbLevels[startingLevelValue - 1]) { 
    if(millis() - nextLevel > 18*currSpeed) {
     if(cntObstacle == 0) {
      if(justStarted == 0 && lost == 0) {
       Sing();
       startingLevelValue++;
      }
      if(lost == 1){
        lost = 0;
      }
      showLevel();
      startScrolling = 0;
      currSpeed -= 20;
      if(startingLevelValue < 6)
       StartGame();
      intermScore = score;
     }
     if(startingLevelValue < 6) {
      if(verifyUpperMatrix() == 1) {
       fillMatrix();
       cntObstacle ++; 
       if(startingLevelValue < 6) {
        score += 10*startingLevelValue;
        StartGame();
       }
      }
     }
    }
    Scroll();
   
    if(millis() - currTime > currSpeed) { 
     shiftMatrix();
     if(verifyHit() == 1) {
      cntObstacle = 0;
      lives--;
      lost = 1; 
      if(lives > 0)
       score = intermScore;
      lostLives(3 - lives);
      for(int row = 13; row >= 0; row--) 
       for(int col = 1; col < 7; col++)
         Matrix[row][col] = 0;
      for(int row = 6; row < 14; row++)
       for(int col = 0; col < 8; col++)
        lc.setLed(0, row - 6, col, false);
      if(lives == 0) {
       losingMessage(); 
      }
     }
      
     shift = 1;
    }
   }
   else {
    nextLevel = millis();
    cntObstacle = 0;
    justStarted = 0;
   }
  }
 }
 else {
  for(int row = 0; row < 8; row++)
   for(int col = 0; col < 8; col++)
    lc.setLed(0, row, col, false);
  
  if(Select() == 1){
   winningMessage();
  }
   
  if(Select() == 0) {
   startingLevelValue = 1;
   message1 = 0;
   message2 = 0;
   message3 = 0;
   message4 = 0;
   startMess = 1;
  }
 }
}
 
