#include <Arduino.h>
#include "WiFi.h"

#include <WiFi.h>

const char *ssid = "Lab107_Fake_AP";
const char *password = "Yaay!EpicPassword";

void setup()
{
  Serial.begin(115200);

  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
}

void loop()
{
}
