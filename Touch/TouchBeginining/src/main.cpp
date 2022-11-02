#include <Arduino.h>

byte touchPin = T0;
void setup() {
  Serial.begin(115200);
  touchSetCycles(0x1000, 0x1000);
}

void loop() {
  Serial.println(touchRead(touchPin));
  delay(100);
}