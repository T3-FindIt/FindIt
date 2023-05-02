#include <Arduino.h>
#include <WiFi.h>

const char *ssid = "SSID";
const char *password = "PASSWORD";

const uint16_t hostPort = 54000;
const char *hostIp = "IP";

void readResponse(WiFiClient *client)
{
    if (client->available() == 0)
    {
        Serial.println("No data available");
            return;
    }

    while (client->available())
    {
        String line = client->readStringUntil('\0'); //\0 because C++ server side is always null terminated.
        Serial.println(line);
    }
}

void setup()
{
    Serial.begin(9600);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.println("...");
    }

    Serial.print("WiFi connected with IP: ");
    Serial.println(WiFi.localIP());
}

void loop()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("...");
        delay(500);
        return;
    }

    WiFiClient client;
    if (!client.connect(hostIp, hostPort))
    {
        Serial.println("Connection to host failed");
        delay(1000);
        return;
    }

    while (true)
    {
    client.print("Hello from ESP32!");
    readResponse(&client);
        delay(500);
    }

    client.stop();
    delay(10000);
}
