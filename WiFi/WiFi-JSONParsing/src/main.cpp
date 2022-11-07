#include <Arduino.h>

#include <WiFi.h>
#include <Update.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* labSSID = "Lab107";
const char* labPass = "Yaay!EpicPassword";

const char *url = "http://puslabosi.getenjoyment.net/ota.json";
const char *host = "puslabosi.getenjoyment.net";
const char *file = "/ota.json";

String devType = "ourESP";
const float currentVersion = 1.0;

const int bufferSize = 512;
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


/* Our JSON Example:

[{
        "id": 1,
        "deviceType": "ourESP",
        "version": 1.2,
        "link": "http://reallinks.com/ourESP.bin"
    },
    {
        "id": 2,
        "deviceType": "notOurESP",
        "version": 1.1,
        "link": "http://reallinks.com/notOurFirmware.bin"
    }
]

*/

String downloadJSON()
{
    String output = "";

    HTTPClient http;

    http.begin(url);
    Serial.println("HTTP GET...");
    int httpCode = http.GET();

    if (httpCode > 0)
    {
        Serial.printf("HTTP GET code: %d\n", httpCode);

        if (httpCode == HTTP_CODE_OK)
            output = http.getString();
    }
    else
        Serial.printf("GET failed, error: %s\n", http.errorToString(httpCode).c_str());

    http.end();

    return output;
}

bool updateAvailable(String json, String &updateLink)
{
    Serial.println(json);
    StaticJsonDocument<384> doc; // https://arduinojson.org/v6/assistant/#/step - Pay attention to the size, 192 per member of this array
    DeserializationError error = deserializeJson(doc, json);

    if (error)
    {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return false;
    }

    for (JsonObject item : doc.as<JsonArray>())
    {
        String currentDeviceType = item["deviceType"].as<String>();
        float newVersion = item["version"];

        if (currentDeviceType == devType && newVersion > currentVersion)
        {
            updateLink = item["link"].as<String>();

            Serial.printf("Update found - current version %f, new version %f", currentVersion, newVersion);

            return true;
        }
    }

    return false;
}

void loop()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        String json = downloadJSON();
        String updateLink = "";

        if (updateAvailable(json, updateLink))
        {
            //...do actual updating
        }
        else
            Serial.println("No update available");
    }
    else
        Serial.println("WiFi not connected" + WiFi.status());

    delay(10000);
}