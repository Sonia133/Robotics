Homework 1 : RGBLed  - RGB LED , each color controlled by one potentiometer.

Homework 2 : KnockDetector - Two buzzers (passive and active) and one button. The passive one detects knocking and changes his value . If the value is over a certain threshold , the active buzzer starts to play tones and stops when the button is pressed .

Homework 3 : 4 Digit 7 SegmentsDisplay - With the X axis from the Joystick I move left-right between the 4 digits and with the Y axis I change one of the digits value from 0 to 9 . While I cycle through the digits , I am unable to modify their values and also , while I am increasing or decreasing a value , I cannot switch to another digit . This works by locking or unlocking on a digit when I press on the Joysticks button.

Homework 4 : GameMenu - In this homework I created a game menu using an LCD to display it on and a joystick to scroll through the menu (with the X axis), select one category (press the button) and return home (press again the button) . The menu consists of 3 categories : Start Game , Settings and High Score. Start Game starts by default with level 0 which increases every 5 seconds. After 10 seconds the game stops (using millis()), shows three messages and the level becomes 0. In Settings the player can select the level he wants (with Y axis, level not lower than 0 , by default 0). In the end, the High Score option saves the greatest score, through all the games, until the Arduino is restarted or disconnected from the computer.

Here is a video of how my homework works : https://drive.google.com/open?id=1sPJTFUcMX3HqdmrBR8Vs-FChn2NTV5Nc 
