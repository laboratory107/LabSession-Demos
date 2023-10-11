#include <Arduino.h>

const int btnPin = 21;
const int ledPin = 23;
int currentState = LOW, lastState = HIGH;

void setup() {
  pinMode(btnPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  currentState = digitalRead(btnPin);

  if(currentState == HIGH && lastState == LOW) {
    digitalWrite(ledPin, LOW);
  } else if (currentState == LOW && lastState == HIGH) {
    digitalWrite(ledPin, HIGH);
  }

  lastState = currentState;
}
