const int speakerPin = A0;
const int buzzerPin = 11;

int buzzerTone = 1000;
int speakerValue = 0;

const int threshold = 1;

const int pushButton = 2;
int buttonState = 0;

int startSong = 0;
int knock = 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(speakerPin, INPUT);
  pinMode(pushButton, INPUT_PULLUP);
  Serial.begin(9600);
} 

void loop() {
  // put your main code here, to run repeatedly:
  speakerValue = analogRead(speakerPin);
  
  if(speakerValue != 0)
  Serial.println(speakerValue);
  
  if(speakerValue > threshold){
    Serial.println("Knock");
    knock = millis();
    if(millis() - knock > 5000){
    startSong = 1;
  }
 }
  if(startSong == 1)
   tone(buzzerPin, buzzerTone);

  buttonState = digitalRead(pushButton);
  
   if(buttonState == LOW && startSong == 1){
    noTone(buzzerPin);
    startSong = 0;
   }
}
