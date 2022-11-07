#include <Arduino.h>

#include <WiFi.h>

#include <HTTPClient.h>

const char* labSSID = "Lab107";
const char* labPass = "Yaay!EpicPassword";

const char* url = "http://puslabosi.getenjoyment.net/ota.json";
const char* host = "puslabosi.getenjoyment.net";
const char* file = "/ota.json";

int timeoutCounter = 0;

void setup()
{
    Serial.begin(115200);

    WiFi.mode(WIFI_STA);
    WiFi.begin(labSSID, labPass);
    Serial.println("Connecting");

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);

        timeoutCounter++;
        if (timeoutCounter >= 20)
            ESP.restart();
    }
}

String getHeaderValue(String header, String headerName)
{
    return header.substring(strlen(headerName.c_str()));
}

WiFiClient client;

bool checkHeader(int &contentLength) {
    while (client.available()) {
        String line = client.readStringUntil('\n');
        line.trim();

        Serial.println(line);
        if (!line.length())  break;

        if (line.startsWith("HTTP/1.1"))
            if (line.indexOf("200") < 0)
                return false;

        if (line.startsWith("Content-Length: "))
            contentLength = atoi((getHeaderValue(line, "Content-Length: ")).c_str());

        if (line.startsWith("Content-Type: "))
        {
            String contentType = getHeaderValue(line, "Content-Type: ");
            if (contentType == "application/octet-stream")
                return false;
        }
    }

    return true;
}

void downloadFile(const char *host, const char *fileName)
{
    int contentLength = 0;
    Serial.println("Attempting connection");
    if (client.connect(host, 80))
    {
        Serial.println("Connected to host");
        client.print(String("GET ") + fileName + " HTTP/1.1\r\n" +
                     "Host: " + host + "\r\n" +
                     "Cache-Control: no-cache\r\n" +
                     "Connection: close\r\n\r\n");
                     
        unsigned long timeout = millis();
        while (client.available() == 0) {
            if (millis() - timeout > 5000) {
                Serial.println("Client Timeout !");
                client.stop();
                return;
            }
        }

        if (checkHeader(contentLength))
        {
            while(client.available()) {
                String line = client.readStringUntil('\r');
                Serial.print(line);
            }
        }
    } 

    client.stop();
}

void loop()
{
    if (WiFi.status() == WL_CONNECTED)
       downloadFile(host, file);
    
    delay(10000);
}
