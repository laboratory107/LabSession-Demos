#include <Arduino.h>

class SuperCool {
private:
    Stream *debugStream;
    bool debugEnabled = false;

public:
    void enableDebug(Stream &debugStream = Serial);
    void blinkLED(int ledPIN);
};