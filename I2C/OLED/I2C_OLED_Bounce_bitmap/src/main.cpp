#include <oled.h>
#include <balls.h>
#include <simulation.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define LED_PIN 2

Adafruit_SSD1306 screen(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); //OLED
Simulation *simulation;
ITask *task;

void setup()
{
  Serial.begin(115200);
  if(!screen.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println("Screen not present.");
    while(1)
      delay(1);
  }
  screen.clearDisplay();
  screen.setTextColor(WHITE);
  screen.setTextSize(0);
  screen.display();

  task = new LED_Task(LED_PIN, 40, false);

  simulation = new Simulation(7,task);

  delay(2000);
}

long t, deltaT;
int tCount = 0;

void loop()
{
  Ball *balls;
  short nBalls;
  float startMomentum, currentMomentum;
  int i;
  screen.clearDisplay();
  simulation->updatePositions(SCREEN_WIDTH, SCREEN_HEIGHT);
  startMomentum = simulation->getStartTotalMomentum();
  currentMomentum = simulation->getCurrentTotalMomentum();
  nBalls = simulation->getBalls(balls);
  
  screen.drawRect(0,0,128,64,WHITE);
  for(i=0; i<nBalls; i++)
  {
    float r = balls[i].radius;
    int bmp = (int)(balls[i].radius-5.0);
    screen.drawBitmap(balls[i].position[0]-r, balls[i].position[1]-r,bitmapPointers[bmp],
    r*2,r*2,WHITE);
    //screen.drawCircle(balls[i].position[0], balls[i].position[1], balls[i].radius, WHITE);

  }
  
  screen.setCursor(0, 1);
  screen.printf("m:%.3f/%.3f", currentMomentum, startMomentum);
  screen.setCursor(74,54);
  screen.printf("%.1f fps", 100000.0/(float)deltaT);
  screen.display();
  tCount++;
  if(tCount == 100)
  {
    deltaT = millis()-t;
    t = millis();
    tCount = 0;
  }
  task->Tick();
}

/* LED Task methods */
LED_Task::LED_Task(int pin, int delay, boolean swap)
{
  this->pin = pin;
  this->delay = delay;
  this->swap = swap;
  pinMode(pin,OUTPUT);
}

void LED_Task::On()
{
  if(!swap)
    digitalWrite(pin,HIGH);
  else
    digitalWrite(pin,LOW);
  t = millis();
}

void LED_Task::Off()
{
    if(!swap)
    digitalWrite(pin,LOW);
  else
    digitalWrite(pin,HIGH);
}

void LED_Task::Tick()
{
  long t1 = millis();
  if((t1-t)>delay)
    Off();
}
