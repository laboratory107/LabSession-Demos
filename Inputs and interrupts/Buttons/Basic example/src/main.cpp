#include <Arduino.h>

const int btnPin = 21;
const int ledPin = 23;
int currentState = LOW, lastState = HIGH;

void setup() {
  pinMode(btnPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  currentState = digitalRead(btnPin);

  if(currentState == HIGH && lastState == LOW) {
    digitalWrite(ledPin, HIGH);
  } else if (currentState == LOW && lastState == HIGH) {
    digitalWrite(ledPin, LOW);
  }

  lastState = currentState;
}
