#include <Arduino.h>

const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;
const int rPin = 23;
const int gPin = 22;
const int bPin = 21;
 
void setup() {
  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(rPin, ledChannel);
  ledcAttachPin(gPin, ledChannel);
  ledcAttachPin(bPin, ledChannel);
}
 
void loop() {
  for (int dutyCycle = 0; dutyCycle <= 255; dutyCycle++) {
    ledcWrite(ledChannel, dutyCycle);
    delay(20);
  } 
}

