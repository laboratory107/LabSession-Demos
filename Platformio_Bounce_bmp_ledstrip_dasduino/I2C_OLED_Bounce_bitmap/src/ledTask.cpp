#include <ledTask.h>

Freenove_ESP32_WS2812 ledRGB = Freenove_ESP32_WS2812(LEDS_COUNT, LEDS_PIN, CHANNEL, TYPE_GRB);
Freenove_ESP32_WS2812 stripRGB = Freenove_ESP32_WS2812(STRIP_COUNT, STRIP_PIN, CHANNEL, TYPE_GRB);

u8 GRBcolors[][3]={{255,0,0},{0,255,0},{0,0,255},{255,255,0},{255,0,255},{0,255,255},{255,255,255},
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
  ledRGB->setAllLedsColorData(0,255,0);
  ledRGB->show();
  t = millis();
}

void LED_Task::On()
{
  ledRGB->setLedColorData(0,GRBcolors[colorCycle][0],GRBcolors[colorCycle][1],GRBcolors[colorCycle][2]);
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
LEDStrip_Task :: LEDStrip_Task(Freenove_ESP32_WS2812* led, u8 brightness, int delay)
                :LED_Task(led, brightness, delay)
{
  int i, j;
  this->ledRGB = led;
  this->delay = delay;
  for(j=0; j<STRIP_COUNT; j++)
    for(i=0; i<3; i++)
      ledColors[j][i]=0;
  colorCycle=0;
  ledRGB->begin();

  ledRGB->setBrightness(brightness);
  for(j=0; j<STRIP_COUNT-1; j++)
    ledRGB->setAllLedsColorData(0,255,0);
  ledRGB->show();
  t = millis();
}

void LEDStrip_Task::Off()
{
  ledRGB->setLedColorData(0,0,0,0);
  ledRGB->show();
  t = millis();
}

void LEDStrip_Task::On()
{
  int i,j;
  for(i=0; i<3; i++)
    ledColors[0][i]=GRBcolors[colorCycle][i];
  ledRGB->setLedColorData(0,ledColors[0][0],ledColors[0][1],ledColors[0][2]);
  ledRGB->show();
  colorCycle++;
  if(colorCycle>=COLORS)
    colorCycle=0;
}

void LEDStrip_Task::Tick()
{
  int i,j;
  long t1;
  for(j=STRIP_COUNT-1; j>0; j--)
  {
    for(i=0; i<3; i++)
      ledColors[j][i]=ledColors[j-1][i];
    ledRGB->setLedColorData(j,ledColors[j][0],ledColors[j][1],ledColors[j][2]);
  }
  for(i=0; i<3; i++)
    ledColors[0][i]=0;
  ledRGB->show();
  t1 = millis();
  if((t1-t)>delay)
    Off();
}


