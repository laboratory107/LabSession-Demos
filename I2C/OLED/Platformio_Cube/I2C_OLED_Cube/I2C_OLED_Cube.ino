#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define LED_PIN 2

#define PI 3.1415927

Adafruit_SSD1306 screen(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); //OLED

int cubeVertice[][3] = {{10,10,-10},{-10,10,-10},{-10,10,10},{10,10,10},{10,-10,-10},{-10,-10,-10},{-10,-10,10},{10,-10,10}};

byte cubeEdges[][2] = {{0,1},{1,2},{2,3},{3,0},{4,5},{5,6},{6,7},{7,4},{0,4},{1,5},{2,6},{3,7}};

float cubeRotation1[8][3];
float cubeRotation2[8][3];
int cubeProjection[8][2];


void setup() 
{
  Serial.begin(115200);
  if(!screen.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println("Screen not present.");
    while(1)
      delay(1);
  }
  delay(1000);
  screen.clearDisplay();
  screen.display();

  pinMode(LED_PIN,OUTPUT);

  Serial.println("OLED screen demo.");

}

long t, deltaT;
int tCount = 0;

void loop() 
{
  static int alpha = 0, beta = 0, gamma = 0;
  digitalWrite(LED_PIN, HIGH);
  drawScene(alpha, beta, gamma);
  digitalWrite(LED_PIN, LOW);
  alpha+=5;
  if(alpha>=360)
    alpha = 0;
  beta+=2;
  if(beta>=360)
    beta = 0;
  gamma+=1;
  if(gamma>=360)
    gamma = 0;
  Serial.printf("FPS: %.1f\n", 100000.0/(float)deltaT);
  tCount++;
  if(tCount == 100)
  {
    deltaT = millis()-t;
    t = millis();
    tCount = 0;
  }
}

void drawScene(int alpha, int beta, int gamma)
{
  const float toRad = 2.0*PI/360.0;
  const float f = 3.0, d = 30.0, z = 20.0;
  const int screenX = 84, screenY = 32;
  int i;
  float fdv;;
  float sinan, cosan;
  screen.setTextSize(1);
  screen.setTextColor(WHITE);
  
  //rotation x
  sinan = sin(toRad*alpha);
  cosan = cos(toRad*alpha);
  for(i=0; i<8; i++)
  {
    cubeRotation1[i][0] = cubeVertice[i][0];
    cubeRotation1[i][1] = cubeVertice[i][1] * cosan - cubeVertice[i][2] * sinan;
    cubeRotation1[i][2] = cubeVertice[i][2] * cosan + cubeVertice[i][1] * sinan;
  }

  //rotation y
  sinan = sin(toRad*beta);
  cosan = cos(toRad*beta);
  for(i=0; i<8; i++)
  {
    cubeRotation2[i][0] = cubeRotation1[i][0] * cosan + cubeRotation1[i][2] * sinan;
    cubeRotation2[i][1] = cubeRotation1[i][1];
    cubeRotation2[i][2] = cubeRotation1[i][2] * cosan - cubeRotation1[i][0] * sinan;
  }  

  //rotation z
  sinan = sin(toRad*gamma);
  cosan = cos(toRad*gamma);
  for(i=0; i<8; i++)
  {
    cubeRotation1[i][0] = cubeRotation2[i][0] * cosan - cubeRotation2[i][1] * sinan;
    cubeRotation1[i][1] = cubeRotation2[i][1] * cosan + cubeRotation2[i][0] * sinan;
    cubeRotation1[i][2] = cubeRotation2[i][2];
  }

  //projection
  for(i = 0; i<8; i++)
  {
    fdv = 1.0/(f+d-cubeRotation1[i][2]);
    cubeProjection[i][0] = z*(float)cubeRotation1[i][0]*f*fdv;
    cubeProjection[i][1] = z*(float)cubeRotation1[i][1]*f*fdv;
  }

  screen.clearDisplay();
  //draw edges
  for(i=0; i<12; i++)
    screen.drawLine(screenX+cubeProjection[cubeEdges[i][0]][0],screenY-cubeProjection[cubeEdges[i][0]][1],screenX+cubeProjection[cubeEdges[i][1]][0],screenY-cubeProjection[cubeEdges[i][1]][1],WHITE);

  //print angles
  screen.setCursor(2, 2);
  screen.printf("alpha:\n %d", alpha);
  screen.setCursor(2, 20);
  screen.printf("beta:\n %d", beta);
  screen.setCursor(2, 38);
  screen.printf("gamma:\n %d", gamma);

  screen.setCursor(2,56);
  screen.printf("%.1f fps",100000.0/(float)deltaT);

  screen.display();
}
