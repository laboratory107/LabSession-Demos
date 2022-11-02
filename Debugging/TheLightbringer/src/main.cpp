#include <Arduino.h>
#include <map>

int led = 21;
std::map<String, int> cmds = {
  {"on", HIGH},
  {"off", LOW}
};

void greeting() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(led, HIGH);
    delay(1000);
    digitalWrite(led, LOW);
    delay(1000);
  }
}

void processCommand(String command)
{
    Serial.println("You asked for light to be: " + command);
    if(command == "on" || command == "off")
    {
      Serial.println("light is now " + command);
      digitalWrite(led, cmds[command]);
    }
    else
    {
      Serial.println("I don't get that :(");
    }
}

void readSerialInput()
{
  String input = "";

  while(Serial.available() > 0)
  {
    byte incoming = Serial.read();
    input.concat((char)incoming);
  }

  if(input != "")
  {
    input.trim();
    processCommand(input);
    input = "";
  }
}

void setup()
{
  pinMode(led, OUTPUT);

  Serial.begin(115200);
  Serial.println("Light bringer v1.0");
  greeting();
  Serial.println("What is it you wish to do?");
}

void loop()
{
  readSerialInput();
}