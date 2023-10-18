#include <Arduino.h>

const int buzzerPin = 5;
const int buttonPin = 2;
 
void setup() {
   pinMode(buzzerPin, OUTPUT);
   pinMode(buttonPin, INPUT_PULLUP);
}
 
void loop() {
   bool buttonState = digitalRead(buttonPin) == LOW;
   digitalWrite(buzzerPin, buttonState ? HIGH : LOW);
}
