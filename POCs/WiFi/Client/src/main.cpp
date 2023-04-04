#include <Arduino.h>
#include <WiFi.h>

const char *ssid = "SSID";
const char *password = "PASSWORD";

const uint16_t hostPort = 8090;
const char *hostIp = "IP";

void readResponse(WiFiClient *client)
{
    unsigned long timeout = millis();
    while (client->available() == 0)
    {
        if (millis() - timeout > 5000)
        {
            Serial.println(">>> Client Timeout !");
            client->stop();
            return;
        }
    }

    // Read all the lines of the reply from server and print them to Serial
    while (client->available())
    {
        String line = client->readStringUntil('\0'); //\0 because C++ server side is always null terminated.
        Serial.println(line);
    }

    Serial.println("Closing connection");
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

    client.print("Hello from ESP32!");
    long rssi = WiFi.RSSI();
    Serial.print("RSSI:");
    Serial.println(rssi);
    readResponse(&client);
    client.stop();
    delay(10000);
}
