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

void loop()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        HTTPClient http;

        Serial.println("HTTP begin...");

        http.begin(url);
        // OR separately host/port/file
        // http.begin("puslabosi.atwebpages.com", 80, "/ota.json");

        Serial.println("[HTTP] GET...");
        int httpCode = http.GET();

        if (httpCode > 0)
        {
            Serial.printf("[HTTP] GET... code: %d\n", httpCode);

            if (httpCode == HTTP_CODE_OK)
            {
                String payload = http.getString();
                Serial.println(payload);
            }
        }
        else
            Serial.printf("GET failed, error: %s\n", http.errorToString(httpCode).c_str());

        http.end();
    }
    else
        Serial.println("WiFi not connected" + WiFi.status());

    delay(10000);
}
