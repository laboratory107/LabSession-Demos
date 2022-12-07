#include <ledTask.h>

Freenove_ESP32_WS2812 ledRGB = Freenove_ESP32_WS2812(LEDS_COUNT, LEDS_PIN, CHANNEL, TYPE_RGB);
Freenove_ESP32_WS2812 stripRGB = Freenove_ESP32_WS2812(STRIP_COUNT, STRIP_PIN, CHANNEL, TYPE_RGB);

u8 BGRcolors[][3]={{255,0,0},{0,255,0},{0,0,255},{255,255,0},{255,0,255},{0,255,255},{255,255,255},
{255,128,128},{128,255,128},{128,128,255},{255,255,128},{255,128,255},{128,255,255},
{255,128,0},{255,0,128},{0,255,128},{128,255,0},{0,128,255},{128,0,255}};

/* LED Task methods */
LED_Task::LED_Task(Freenove_ESP32_WS2812* led, u8 brightness, int delay)
{
  this->ledRGB = led;
  this->delay = delay;
  colorCycle = 0;
  ledRGB->begin();
  ledRGB->setBrightness(brightness);
  ledRGB->setAllLedsColorData(255,0,0);
  ledRGB->show();
  t = millis();
}

void LED_Task::On()
{
  ledRGB->setLedColorData(0,BGRcolors[colorCycle][0],BGRcolors[colorCycle][1],BGRcolors[colorCycle][2]);
  ledRGB->show();
  colorCycle++;
  if(colorCycle>=COLORS)
    colorCycle=0;
  t = millis();
}

void LED_Task::Off()
{
  ledRGB->setAllLedsColorData(0,0,0);
  ledRGB->show();
  t = millis();
}

void LED_Task::Tick()
{
  long t1 = millis();
  if((t1-t)>delay)
    Off();
}

/* LED strip task methods */
LEDStrip_Task :: LEDStrip_Task(Freenove_ESP32_WS2812* led, u8 brightness)
{
  int i, j;
  this->stripRGB = led;
  for(j=0; j<STRIP_COUNT; j++)
    for(i=0; i<3; i++)
      ledColors[j][i]=0;
  colorCycle=0;
  stripRGB->begin();

  stripRGB->setBrightness(brightness);
  for(j=0; j<STRIP_COUNT-1; j++)
    stripRGB->setAllLedsColorData(255,0,0);
  stripRGB->show();
}

void LEDStrip_Task::On()
{
  int i,j;
  for(i=0; i<3; i++)
    ledColors[0][i]=BGRcolors[colorCycle][i];
  stripRGB->setLedColorData(0,ledColors[0][0],ledColors[0][1],ledColors[0][2]);
  stripRGB->show();
  colorCycle++;
  if(colorCycle>=COLORS)
    colorCycle=0;
}

void LEDStrip_Task::Tick()
{
  int i,j;
  for(j=STRIP_COUNT-1; j>0; j--)
  {
    for(i=0; i<3; i++)
      ledColors[j][i]=ledColors[j-1][i];
    stripRGB->setLedColorData(j,ledColors[j][0],ledColors[j][1],ledColors[j][2]);
  }
  for(i=0; i<3; i++)
    ledColors[0][i]=0;
  stripRGB->setLedColorData(0,ledColors[0][0],ledColors[0][1],ledColors[0][2]);
  stripRGB->show();
}

