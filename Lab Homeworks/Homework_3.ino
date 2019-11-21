const int pinSW = 1;
const int pinX = A0;
const int pinY = A1;
int xValue = 0;
int yValue = 0;

bool joyMovedY = false;
bool joyMovedX = false;
int digit = 0;
int minThreshold = 400;
int maxThreshold = 600;

const int pinA = 12;
const int pinB = 8;
const int pinC = 5;
const int pinD = 3;
const int pinE = 2;
const int pinF = 11;
const int pinG = 6;

const int pinDP = 4;
const int pinD1 = 7;
const int pinD2 = 9;
const int pinD3 = 10;
const int pinD4 = 13;

const int segSize = 8;
const int noOfDisplays = 4;
const int noOfDigits = 10;

int swState = LOW;
int lastSwState = LOW;

int allow = HIGH;

int segments[segSize] = {
 pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};

int currDigit = 3;
int valueD1 = 0;
int valueD2 = 0;
int valueD3 = 0;
int valueD4 = 0;

int values[noOfDisplays] = {
 valueD1, valueD2, valueD3, valueD4  
};

int digits[noOfDisplays] = {
 pinD1, pinD2, pinD3, pinD4
};

byte digitMatrix[noOfDigits][segSize - 1] = {
{1, 1, 1, 1, 1, 1, 0}, // 0
{0, 1, 1, 0, 0, 0, 0}, // 1
{1, 1, 0, 1, 1, 0, 1}, // 2
{1, 1, 1, 1, 0, 0, 1}, // 3
{0, 1, 1, 0, 0, 1, 1}, // 4
{1, 0, 1, 1, 0, 1, 1}, // 5
{1, 0, 1, 1, 1, 1, 1}, // 6
{1, 1, 1, 0, 0, 0, 0}, // 7
{1, 1, 1, 1, 1, 1, 1}, // 8
{1, 1, 1, 1, 0, 1, 1} // 9
};

void displayNumber(byte digit, byte decimalPoint) {
 for (int i = 0; i < segSize - 1; i++) 
  digitalWrite(segments[i], digitMatrix[digit][i]);


 digitalWrite(segments[segSize - 1], decimalPoint);
}

void showDigit(int num) {
 for (int i = 0; i < noOfDisplays; i++) {
  digitalWrite(digits[i], HIGH);
 }
  digitalWrite(digits[num], LOW);
}

void setup() {

 for (int i = 0; i < segSize; i++){
  pinMode(segments[i], OUTPUT);
 }
 for (int i = 0; i < noOfDisplays; i++){
  pinMode(digits[i], OUTPUT);
 }

  pinMode(pinSW, INPUT_PULLUP);
     
 Serial.begin(9600);
 
}

void loop() {

  
  swState = digitalRead(pinSW);
  if (swState !=  lastSwState) {
  if (swState == LOW) {
      allow = !allow;
   }
  }
  lastSwState = swState;

 if (allow == HIGH){
  xValue = analogRead(pinX);
  if (xValue > maxThreshold && joyMovedX == false){
   if (currDigit > 0) {
     currDigit--;
     } 
     else {
      currDigit = 3;
     }
    joyMovedX = true;
   }
 
  if (xValue < minThreshold && joyMovedX == false){
  if (currDigit < 3) {
      currDigit++;
  } else {
      currDigit = 0;
  }
  joyMovedX = true;
  }

  if (xValue >= minThreshold && xValue <= maxThreshold) {
  joyMovedX = false;
  }
 }

 if (allow == LOW){
  yValue = analogRead(pinY);
  
  if (yValue > maxThreshold && joyMovedY == false) {
  if (values[currDigit] > 0) {
      values[currDigit]--;
  } else {
      values[currDigit] = 9;
  }
  joyMovedY = true;
  }
   
  if (yValue < minThreshold && joyMovedY == false) {
  if (values[currDigit] < 9) {
      values[currDigit]++;
  } else {
      values[currDigit] = 0;
  }
  joyMovedY = true;
  }

  if (yValue >= minThreshold && yValue <= maxThreshold) {
  joyMovedY = false;
  }
 }

 for(int i = 0; i < noOfDisplays; i++){
  showDigit(i);
  displayNumber(values[i], HIGH);
  if(i == currDigit){
   digitalWrite(segments[segSize - 1], HIGH);
  }
   else digitalWrite(segments[segSize - 1], LOW);
  delay(5);
 }
 
}
