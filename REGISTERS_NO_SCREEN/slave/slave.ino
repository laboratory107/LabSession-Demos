#include <Wire.h>
#include <mutex>
#define SDA0_Pin 21
#define SCL0_Pin 22
#define I2C_Freq 100000
#define I2C_DEV_ADDR 0x10
#define LED_PIN 5 //ESP32

typedef struct
{
  int reqCount;
  int reg1;
  byte reg2;
  char reg3;
  double reg4;
  char reg5[5];
} __attribute__((packed)) reg;

typedef struct
{
  int requestCount;
  byte command;
} __attribute__((packed)) req;

void onReceive(int size);
void onRequest();

std::mutex regMutex;
reg registers;
req requestCommand;

void setup() 
{
  Serial.begin(115200);
  if(!Wire.begin((uint8_t)I2C_DEV_ADDR, SDA0_Pin, SCL0_Pin, I2C_Freq))
  {
    Serial.println("I2C Wire Error. Going idle.");
    while(true)
      delay(1);
  }
  Wire.onReceive(onReceive);
  Wire.onRequest(onRequest); 
  randomSeed(0);
  updateRegisters();
  pinMode(LED_PIN,OUTPUT);
  Serial.println("Slave engaged.");
  delay(500);
}

void loop() 
{
  digitalWrite(LED_PIN, HIGH);
  updateRegisters();
  delay(40);
  digitalWrite(LED_PIN, LOW);
  delay(60);
}

void updateRegisters()
{
  int i;
  std::lock_guard<std::mutex> lock(regMutex);
  registers.reg1 = 55500+random(0,99);
  registers.reg2 = 250+random(0,5);
  registers.reg3 = random(97,122);
  registers.reg4 = 22.0+(double)random(0,1000)/1000.0;
  for(i=0; i<4; i++)
    registers.reg5[i] = random(97,122);
  registers.reg5[i] = '\0';
}

void onReceive(int size)
{
  if(size==sizeof(req))
    Wire.readBytes((char *)&requestCommand, sizeof(req));
  {
    std::lock_guard<std::mutex> lock(regMutex);
    registers.reqCount = requestCommand.requestCount;
  }
}
void onRequest()
{
  std::lock_guard<std::mutex> lock(regMutex);
  switch(requestCommand.command)
  {
    case 0: Wire.write((uint8_t *)&registers.reqCount, sizeof(int)); break;
    case 1: Wire.write((uint8_t *)&registers.reg1, sizeof(int)); break;
    case 2: Wire.write(registers.reg2); break;
    case 3: Wire.write(registers.reg3); break;
    case 4: Wire.write((uint8_t *)&registers.reg4, sizeof(double)); break;
    case 5: Wire.write((uint8_t *)registers.reg5,5); break;
    default: Wire.write(0); //single byte for dummy
  }
}
