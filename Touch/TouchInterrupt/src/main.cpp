#include <Arduino.h>

byte touchPin = T0;
short threshold = 20;
int elapsed = 0;
int sinceLastTouch = 0;
short touchDelay = 350;
bool touched = false;

void IRAM_ATTR touchInterrupt()
{
  touched = true;
}

void setup() {
  Serial.begin(115200);
  touchAttachInterrupt(touchPin, touchInterrupt, threshold);
}

void loop() {
  if (touched)
  {
    touched = false;

    elapsed = millis() - sinceLastTouch;
    if (elapsed > touchDelay)
    {
      Serial.println("Touch sensor touched");
      sinceLastTouch = millis();
    }
  }
}