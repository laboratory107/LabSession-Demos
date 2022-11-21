#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define LED_PIN 2

/* task interface */
class ITask
{
  public:
    virtual void On()=0;
    virtual void Off()=0;
    virtual void Tick()=0;
};


struct Ball
{
  float position[2];
  float speed[2];
  float radius;
  float mass;
};

class Simulation
{
  public:
    Simulation(int nBalls, int radius, ITask *task);
    ~Simulation();
    void updatePositions(int w, int h);
    short getBalls(Ball *&balls);
    float getStartTotalMomentum();
    float getCurrentTotalMomentum();

  private:
    ITask *task;
    short nBalls;
    float startTotalMomentum, currentTotalMomentum;
    Ball *balls;
    bool isInterractionCandidate(Ball *b1, Ball *b2);
    void wallBounce(int w, int h);
    void positionCorrection(Ball *b1, Ball *b2);
    void ballBounce(Ball *b1, Ball *b2, double r);
    void calculateTotalMomentum();
    void keepTotalMomentum();
};

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

  simulation = new Simulation(5,3,task);

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
  
  for(i=0; i<nBalls; i++)
    screen.drawCircle(balls[i].position[0], balls[i].position[1], balls[i].radius, WHITE);
  
  screen.setCursor(0, 0);
  screen.printf("m:%.3f/%.3f, %.0f fps", currentMomentum, startMomentum, 100000.0/(float)deltaT);
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

/* Simulation class methods */

Simulation::Simulation(int nBalls, int radius, ITask *task)
{
    int i;
    this->nBalls = nBalls;
    this->task = task;
    balls = new Ball[nBalls];
    startTotalMomentum = 0.0;
    currentTotalMomentum = 0.0;
    srand(time(0));
    // start condition
    for(i=0; i<nBalls; i++)
    {
        balls[i].radius = radius/2.0+radius*(100+rand()%200)/100.0;
        if(i==0)
        {
            balls[i].position[0]=balls[i].radius;
            balls[i].position[1]=balls[i].radius;
        }
        else
        {
            balls[i].position[0]=balls[i-1].position[0]+balls[i-1].radius+balls[i].radius;
            balls[i].position[1]=balls[i-1].position[1]+balls[i-1].radius+balls[i].radius;
        }
        balls[i].speed[0]=0.5;
        balls[i].speed[1]=0.73;
        balls[i].mass = balls[i].radius*balls[i].radius*0.001;
        currentTotalMomentum+=
            balls[i].speed[0]*balls[i].mass*balls[i].speed[0]*balls[i].mass+
            balls[i].speed[1]*balls[i].mass*balls[i].speed[1]*balls[i].mass;
    }
    calculateTotalMomentum();
    startTotalMomentum = currentTotalMomentum;
}

Simulation::~Simulation()
{
  delete[] balls;
}

void Simulation::updatePositions(int w, int h)
 {
  int i, j;
  float d;
  wallBounce(w, h);
  for(j=0; j<nBalls; j++)
      for(i=0; i<2; i++)
      {
          float r = balls[j].radius + balls[i].radius;
          balls[j].position[i]+=balls[j].speed[i];
      }
  for(j=0; j<nBalls-1; j++)
      for(i=j+1; i<nBalls; i++)
          if(isInterractionCandidate(balls+j, balls+i))
          {
              float r = balls[j].radius + balls[i].radius;
              float dx = balls[i].position[0] - balls[j].position[0];
              float dy = balls[i].position[1] - balls[j].position[1];
              d = sqrt(dx*dx+dy*dy);
              if(d<=r)
              {
                  positionCorrection(balls+j,balls+i);
                  ballBounce(balls+j,balls+i,r);
                  keepTotalMomentum();
              }
          }
  calculateTotalMomentum();
 }

void Simulation::wallBounce(int w, int h)
{
    int i;
    for(i=0; i<nBalls; i++)
    {
        if(balls[i].position[0]<balls[i].radius && balls[i].speed[0]<0.0)
            balls[i].speed[0]=-balls[i].speed[0];
        if(balls[i].position[1]<balls[i].radius && balls[i].speed[1]<0.0)
            balls[i].speed[1]=-balls[i].speed[1];
        if(balls[i].position[0]>w-balls[i].radius && balls[i].speed[0]>0.0)
            balls[i].speed[0]=-balls[i].speed[0];
        if(balls[i].position[1]>h-balls[i].radius && balls[i].speed[1]>0.0)
            balls[i].speed[1]=-balls[i].speed[1];
    }
}

bool Simulation::isInterractionCandidate(Ball *b1, Ball *b2)
{
  bool br[2];
  int i;
  float r = b1->radius + b2->radius;
  for(i=0; i<2; i++)
      if(abs(b2->position[i]-b1->position[i])<=r)
          br[i] = true;
      else
          br[i] = false;
  return br[0]&&br[1];
}

void Simulation::positionCorrection(Ball *b1, Ball *b2)
{
  int i;
  float corr = 0.0;
  float deltaPos[2];
  float r = b1->radius + b2->radius;
  for(i=0; i<2; i++)
  {
      deltaPos[i] = b2->position[i]-b1->position[i];
      corr+=deltaPos[i]*deltaPos[i];
  }
  corr = r/sqrt(corr); //ratio distance/real distance
  for(i=0; i<2; i++)
      b2->position[i]=b1->position[i]+deltaPos[i]*corr;
}

void Simulation::ballBounce(Ball *b1, Ball *b2, double r)
{
  int i;
  float deltaPos[2], deltaSpeed[2], speedr;
  float vr = 0.0;
  task->On();
  for(i=0; i<2; i++)
  {
      deltaPos[i] = b2->position[i]-b1->position[i];
      deltaSpeed[i] = b2->speed[i]-b1->speed[i];
      vr+=deltaPos[i]*deltaSpeed[i]; //scalar projection to distance vector
  }
  // vr /=r; scalar speed, divide with length od distance vector speed * cos (theta)
  vr /= (r*r); //divide again to be able to multiply with position vector to get vector
  if(vr<0) //bounce
  {
      float M = b1->mass+b2->mass; //mass of both balls
      //double x = vr/r; //speed to distance vector length
      for(i=0; i<2; i++)
      {
          speedr=deltaPos[i]*vr; //speed change along distance vector by components
          deltaSpeed[i]+=speedr*((b2->mass-b1->mass)/M-1);
          //same as deltaSpeed[i]-=speedr and deltaSpeed[i]+=speedr*(b2->mass-b1->mass)/M
          b2->speed[i]=b1->speed[i]+deltaSpeed[i]; //return to lab system
          b1->speed[i]=b1->speed[i]+speedr*2.0*b2->mass/M; //*2.0*b2->mass/M; //momentum ball 1
      }  
  }
}

void Simulation::calculateTotalMomentum()
{
    int i;
    currentTotalMomentum = 0.0;
    for(i=0; i<nBalls; i++)
        currentTotalMomentum+=
            balls[i].speed[0]*balls[i].mass*balls[i].speed[0]*balls[i].mass+
            balls[i].speed[1]*balls[i].mass*balls[i].speed[1]*balls[i].mass;
    currentTotalMomentum = sqrt(currentTotalMomentum);
}

void Simulation::keepTotalMomentum()
{
    int i,j;
    float ratio = startTotalMomentum/currentTotalMomentum;
    for(j=0; j<nBalls; j++)
        for(i=0; i<2; i++)
            balls[j].speed[i]*=ratio;
}

short Simulation::getBalls(Ball *&balls)
{
  int i;
  balls = this->balls;
  return nBalls;
}

float Simulation::getStartTotalMomentum()
{
    return startTotalMomentum;
}

float Simulation::getCurrentTotalMomentum()
{
    return currentTotalMomentum;
}

/* LED Task */
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
