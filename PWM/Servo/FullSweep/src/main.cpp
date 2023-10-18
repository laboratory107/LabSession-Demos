#include <Arduino.h>

const int servoPin = 4, servoMinPos = 13, servoMaxPos = 25;
int currentPos;

void setup()
{
  pinMode(servoPin, OUTPUT);
  analogWriteFrequency(50);
}

void loop()
{
  for (currentPos = servoMinPos; currentPos < servoMaxPos; currentPos++)
  {
    analogWrite(servoPin, currentPos);
    delay(20);
  }

 delay(1000);

  for (currentPos = servoMaxPos; currentPos >= servoMinPos; currentPos--)
  {
    analogWrite(servoPin, currentPos);
    delay(20);
  }

 delay(1000);
}