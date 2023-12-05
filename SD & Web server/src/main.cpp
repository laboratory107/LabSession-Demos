#include <Arduino.h>
#include <SDLogger.h>
#include <WebServer.h>

SDLogger sdLog;
WebServer server;

const char* ssid = "";
const char* password = "";

void setup() {
	Serial.begin(115200);

	if (!sdLog.begin("/log.txt")) {
		Serial.println("No log file!");
		return;
	}

  server.begin("smartlock", "/WebManager", ssid, password, &sdLog);
  server.start();
}

void loop() {
	// put your main code here, to run repeatedly:
}
