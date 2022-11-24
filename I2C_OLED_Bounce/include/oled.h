#ifndef OLED_H
#define OLED_H
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/* task interface */
class ITask
{
  public:
    virtual void On()=0;
    virtual void Off()=0;
    virtual void Tick()=0;
};

/* LED task derived from ITask pure abstract class */
class LED_Task : public ITask
{
  public:
    LED_Task(int pin, int delay, boolean swap);
    void On();
    void Off();
    void Tick();
  private:
    bool swap;
    long t;
    int pin;
    int delay;
};

#endif