#include <SuperCoolLibrary.h>

SuperCool lib;
void setup() {
  Serial.begin(115200);
  lib.enableDebug(Serial);
}

void loop() {
  lib.blinkLED(21);
}