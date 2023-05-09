#include <Arduino.h>
#include <WiFi.h>
#include <string>

const char *ssid = "SSID";
const char *password = "PASSWORD";

const uint16_t hostPort = 54000;
const char *hostIp = "IP";

std::string readResponse(WiFiClient *client)
{
    if (client->available() == 0)
    {
        Serial.println("No data available");
            return "";
    }

    std::string line = "";
    while (client->available())
    {
        line = std::string(client->readStringUntil('\0').c_str());
        // line = client->readStringUntil('\0'); //\0 because C++ server side is always null terminated.
        Serial.println(line.c_str());
    }
    return line;
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
    delay(1000);
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

    int i = 0;

    while (i < 50)
    {
        // client.print("Hello from ESP32!");
        std::string msg = readResponse(&client);
        if (msg == "{\"action\": \"HeartBeat\"}")
            client.print("{\"action\": \"HeartBeat\",\"Node\": \"Test\",\"Places\": \"5\"}\0");
        delay(500);
        i++;
    }

    client.stop();
    delay(1000);
}
