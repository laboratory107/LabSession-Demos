#include <Arduino.h>

volatile bool buttonPressed = false;



const int ledPin = 23;
const int buttonPin = 21;
 
int buttonState = LOW;
bool ledState = false, lastState = true;
 
long lastDebounceTime = 0; 
long debounceDelay = 50;    

void IRAM_ATTR ISR() {
    buttonPressed = true;
}

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  attachInterrupt(buttonPin, ISR, FALLING);
}



void loop() { 
  if (buttonPressed == HIGH && lastState == LOW) {
      digitalWrite(ledPin, HIGH);
      lastDebounceTime = millis(); 
      buttonPressed = false;
      lastState = true;
  } else if (buttonPressed == LOW && lastState == HIGH) { 
      digitalWrite(ledPin, LOW); 
      lastDebounceTime = millis();
      lastState = false;
  }
}


