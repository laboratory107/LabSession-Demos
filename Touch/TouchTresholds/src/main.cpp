#include <Arduino.h>

short threshold = 20;
byte touchPin = T0;
int elapsed = 0;
int sinceLastTouch = 0;
int touchDelay = 500;

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  if (touchRead(touchPin) < threshold)
  {
    elapsed = millis() - sinceLastTouch;
    if (elapsed > touchDelay)
    {
      Serial.println("Touch sensor touched");
      sinceLastTouch = millis();
    }
  }
  
  delay(100);
}