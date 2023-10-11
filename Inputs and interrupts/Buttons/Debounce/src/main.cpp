#include <Arduino.h>

const int ledPin = 21;
const int buttonPin = 23;
 
int currentState = LOW, lastState = HIGH;
 
long lastDebounceTime = 0; 
long debounceDelay = 50;    
 
void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
}
 
void loop() {
  currentState = digitalRead(buttonPin);
 
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (currentState == HIGH && lastState == LOW) {
      digitalWrite(ledPin, HIGH);
      lastDebounceTime = millis(); 
    } else if (currentState == LOW && lastState == HIGH) {
      digitalWrite(ledPin, HIGH); 
      lastDebounceTime = millis();
    }
  }

  lastState = currentState;
}
