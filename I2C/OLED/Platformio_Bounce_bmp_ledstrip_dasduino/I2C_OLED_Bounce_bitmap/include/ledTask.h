#ifndef LEDTASK_H
#define LEDTASK_H

#include <Freenove_WS2812_Lib_for_ESP32.h>
#include <FastLED.h>


#define COLORS 19 

/* RGB LED WS 2812 */
#define LEDS_COUNT  1 //for LED strip, we have only one
#define LEDS_PIN	32 //dasduino RGB LED pin
#define CHANNEL		0 //RGB channel 0

#define STRIP_COUNT 24 //24 diodes in strip
#define STRIP_PIN 15


/* task interface */
class ITask
{
  public:
    virtual void On()=0;
    virtual void Off()=0;
    virtual void Tick()=0;
};

/* LED task derived from ITask pure abstract class */
class LED_Task : virtual public ITask
{
  public:
    LED_Task(Freenove_ESP32_WS2812* led, u8 brightness, int delay);
  private:
    void On();
    void Off();
    void Tick();
  protected:
    short colorCycle;
    long t;
    int delay;
    Freenove_ESP32_WS2812 *ledRGB;
};

class LEDStrip_Task : public LED_Task, virtual public ITask
{
  public:
    LEDStrip_Task(Freenove_ESP32_WS2812* led, u8 brightness, int delay);
    void On();
    void Off();
    void Tick();
  private:
    u8 ledColors[STRIP_COUNT][3];
};

#endif