#include <Arduino.h>
#include <SPI.h>
#include <SSD_13XX.h>

#define __CS    17  
#define __DC    16 
#define __RST   5
//#define __mosi 23
//#define __sclk 18

int cubeVertice[][3] = {{10,10,-10},{-10,10,-10},{-10,10,10},{10,10,10},{10,-10,-10},{-10,-10,-10},{-10,-10,10},{10,-10,10}};
short cubeFaces[][4] = {{0,1,2,3},{3,2,6,7},{0,3,7,4},{0,4,5,1},{1,5,6,2},{4,7,6,5}};

float cubeRotation1[8][3];
float cubeRotation2[8][3];
int cubeProjection[8][2];

void drawScene(int alpha, int beta, int gamma);

SSD_13XX screen = SSD_13XX(__CS,__DC,__RST);

void setup() 
{
  Serial.begin(115200);
  screen.begin();
  delay(1000);
  srand(time(0));
}

long t, deltaT;
int tCount = 0;

void loop() 
{
  static int alpha = 0, beta = 0, gamma = 0;
  drawScene(alpha, beta, gamma);
  alpha+=5;
  if(alpha>=360)
    alpha = 0;
  beta+=2;
  if(beta>=360)
    beta = 0;
  gamma+=1;
  if(gamma>=360)
    gamma = 0;
  delay(40);
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
  const float f = 5.0, d = 60.0, z = 20.0;
  const int screenX = 48, screenY = 32;
  int i,j;
  float fdv;;
  float sinan, cosan;
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

  //draw faces
  float e[3] = {0.0, 0.0, f+d};
  screen.clearScreen();
  for(j=0; j<6; j++)
  {
    float c[3];
    float ce[3];
    float sk = 0.0, cel=0.0, cl=0.0;
    for(i=0; i<3; i++)
    {
      c[i] = (cubeRotation1[cubeFaces[j][0]][i]+cubeRotation1[cubeFaces[j][2]][i])/2.0;
      ce[i]= e[i]-c[i];
      sk+=c[i]*ce[i];
      cel+=ce[i]*ce[i];
      cl+=c[i]*c[i];
    }
    if(sk>0.0)
    {
      sk = sk/(sqrt(cel)*sqrt(cl));
      screen.fillTriangle(screenX+cubeProjection[cubeFaces[j][0]][0],screenY+cubeProjection[cubeFaces[j][0]][1],
      screenX+cubeProjection[cubeFaces[j][1]][0],screenY+cubeProjection[cubeFaces[j][1]][1],
      screenX+cubeProjection[cubeFaces[j][2]][0],screenY+cubeProjection[cubeFaces[j][2]][1],screen.Color565(0,0,sk*255));
      screen.fillTriangle(screenX+cubeProjection[cubeFaces[j][0]][0],screenY+cubeProjection[cubeFaces[j][0]][1],
      screenX+cubeProjection[cubeFaces[j][2]][0],screenY+cubeProjection[cubeFaces[j][2]][1],
      screenX+cubeProjection[cubeFaces[j][3]][0],screenY+cubeProjection[cubeFaces[j][3]][1],screen.Color565(0,0,sk*255));
    }
  }

  screen.setCursor(2,56);
  screen.setTextColor(RED);
  screen.printf("%.1f fps",100000.0/(float)deltaT);

}