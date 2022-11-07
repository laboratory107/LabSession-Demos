#include <Arduino.h>

#include <WiFi.h>

#include <HTTPClient.h>

const char* labSSID = "Lab107";
const char* labPass = "Yaay!EpicPassword";

const char* url = "http://puslabosi.getenjoyment.net/ota.json";
const char *firmwareUrl = "http://puslabosi.getenjoyment.net/firmware.bin";
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
    if ((WiFi.status() == WL_CONNECTED))
    {
        HTTPClient http;

        http.begin(url);

        int httpCode = http.GET();
        if (httpCode > 0)
        {
            if (httpCode == HTTP_CODE_OK)
            {
                int len = http.getSize();

                byte buff[bufferSize] = {0};

                WiFiClient *stream = http.getStreamPtr();

                Serial.println("Download size: " + len);
                while (http.connected() && (len > 0 || len == -1))
                {
                    size_t size = stream->available();

                    if (size)
                    {
                        int c = stream->readBytes(buff, ((size > bufferSize) ? bufferSize : size));

                        // WARNING: only uncomment for smaller files, printing the whole firmware may crash the ESP along with being unreadable 
						// Serial.write(buff, c);

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

    delay(10000);
}
