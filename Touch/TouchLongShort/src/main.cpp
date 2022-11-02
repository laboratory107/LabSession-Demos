#include <Arduino.h>
#include "TouchControl.h"

TouchControl touchControl;

void setup() {
  Serial.begin(115200);
  
  touchControl.enableDebug();
  touchControl.setupTouchInterrupt();
}

void loop() {
  std::string output = touchControl.handleTouch();
  if (output != "No pattern")
    Serial.println(output.c_str());
}