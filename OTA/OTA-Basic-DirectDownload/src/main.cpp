#include <Arduino.h>

#include <WiFi.h>
#include <WiFiMulti.h>
#include <Update.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

const char *labSSID = "437";
const char *labPass = "sifra556";

String devType = "ourESP";
const float currentVersion = 1.0;

const int bufferSize = 512;
int timeoutCounter = 0;

const char *mqttBroker = "broker.hivemq.com";
const char *mqttTopic = "esp32/update";
const char *clientName = "randomEsp";
const int mqttPort = 1883;

HTTPClient http;
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

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

String downloadJSON(String jsonURL)
{
    String output = "";

    http.begin(jsonURL);

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

            Serial.printf("Update found - current version %f, new version %f\n", currentVersion, newVersion);

            return true;
        }
    }

    return false;
}

void updateFirmware(uint8_t *data, size_t len, int &currentLength, int &totalLength)
{
    Update.write(data, len);
    currentLength += len;
    Serial.print('.');

    if (currentLength == totalLength)
    {
        Serial.printf("\nUpdate Success, Total Size: %u\nRebooting...\n", currentLength);

        Update.end(true);
        ESP.restart();
    }
}

void startOTA(String firmwareUrl)
{
    http.begin(firmwareUrl);

    int httpCode = http.GET();
    if (httpCode > 0)
    {
        if (httpCode == HTTP_CODE_OK)
        {
            int len = http.getSize();

            byte buff[bufferSize] = {0};

            WiFiClient *stream = http.getStreamPtr();

            bool canBegin = Update.begin(len);

            int totalSize = len;
            int currentSize = 0;
            int lastBlockSize = 0;
            while (http.connected() && (len > 0 || len == -1))
            {
                size_t size = stream->available();

                if (size)
                {
                    lastBlockSize = size > bufferSize ? bufferSize : size;
                    int c = stream->readBytes(buff, lastBlockSize);

                    updateFirmware(buff, lastBlockSize, currentSize, totalSize);

                    if (len > 0)
                        len -= c;
                }
                delay(1);
            }
        }
    }
    else
        Serial.printf("GET failed - error: %s\n", http.errorToString(httpCode).c_str());

    http.end();
}


void update(String jsonUrl)
{
    if (WiFi.status() == WL_CONNECTED)
    {
        String json = downloadJSON(jsonUrl);
        String updateLink = "";

        if (updateAvailable(json, updateLink))
            startOTA(updateLink);
        else
            Serial.println("No update available");
    }
    else
        Serial.println("WiFi not connected" + WiFi.status());
}

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
   update("http://puslabosi.atwebpages.com/ota.json");
   delay(10000);
}