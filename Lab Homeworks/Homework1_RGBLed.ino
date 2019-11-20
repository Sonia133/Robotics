const int ledRed = 3;
const int ledGreen = 5;
const int ledBlue = 11;

const int potPinR = A0;
const int potPinG = A1;
const int potPinB = A2;

void setup() {
  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledBlue, OUTPUT);

  pinMode(potPinR, INPUT);
  pinMode(potPinG, INPUT);
  pinMode(potPinB, INPUT);
}

void loop() {
  int potValR = analogRead(potPinR);
  int potValG = analogRead(potPinG);
  int potValB = analogRead(potPinB);
  
  int valR = map(potValR, 0, 1023, 0, 255);
  int valG = map(potValG, 0, 1023, 0, 255);
  int valB = map(potValB, 0, 1023, 0, 255);
  
  analogWrite(ledRed, valR);
  analogWrite(ledGreen, valG);
  analogWrite(ledBlue, valB);
}
