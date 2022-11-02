#include "SuperCoolLibrary.h"

void SuperCool::enableDebug(Stream &stream)
{
    debugStream = &stream;
    debugEnabled = true;
}

void SuperCool::blinkLED(int ledPIN)
{
    pinMode(ledPIN, OUTPUT);

    for (int i = 0; i < 5; i++)
    {
        if (debugEnabled)
            debugStream->println("Turning LED on"); // Pay attention

        digitalWrite(ledPIN, HIGH);
        delay(500);

        if (debugEnabled)
            debugStream->println("Turning LED off");

        digitalWrite(ledPIN, LOW);
        delay(500);
    }
}