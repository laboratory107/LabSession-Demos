#include <Arduino.h>

const int rPin = 23;
const int gPin = 22;
const int bPin = 21;
 
void setup() {
  pinMode(rPin, OUTPUT);
  pinMode(gPin, OUTPUT);
  pinMode(bPin, OUTPUT);
}
 
void loop() {
  for (int dutyCycle = 0; dutyCycle  < 255; dutyCycle ++) {
     analogWrite(rPin, dutyCycle);
     delay(20);
  }
}
