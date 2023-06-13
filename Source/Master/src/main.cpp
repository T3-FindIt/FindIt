#include <Arduino.h>

#include "I2C.hpp"
#include "WiFiHandler.hpp"
#include "WebSocketHandler.hpp"

#define LOCAL_ADDRESS 8 // slave address

// Might be better to put this in a secret.h file. To be discussed with the Git Master
#define SSID "SSID"
#define PASSWORD "PASSWORD"

#define SERVER_ADDRESS "SERVER IP"
#define SERVER_PORT 80

#define I2C_
#define WIFI

WiFiHandler wifiHandler = WiFiHandler();
WebSocketHandler webSocketHandler = WebSocketHandler();
I2C i2c (LOCAL_ADDRESS);

#define SCAN_OFFSET 200 * 1000 // 10 Seconds
int scanTime = 0;

void setup()
{
    #ifdef WIFI

    WiFiData wifiData(SSID, PASSWORD);
    wifiHandler = WiFiHandler(wifiData);

    WebSocketData webSocketData(SERVER_ADDRESS, SERVER_PORT);
    webSocketHandler = WebSocketHandler(webSocketData);

    wifiHandler.Connect();
    if (!wifiHandler.isConnected())
    {
        return;
    }

    webSocketHandler.Connect();
    if (!webSocketHandler.isConnected())
    {
        return;
    }
    #endif
    Serial.begin(9600);
    Serial.println();
    Serial.println("Starting up!");
    #ifdef I2C_
    scanTime = millis() + SCAN_OFFSET;
    i2c.InitializeAddresses();
    #endif
}

int lastRequest;


void loop()
{
    #ifdef WIFI
    if (webSocketHandler.isConnected())
    {
        std::string websocketData = webSocketHandler.Recieve();
        if (websocketData != "")
        {
        // Decompile with JSON parser
        // Do some stuff with the data

            Serial.println(websocketData.c_str());
        }

        webSocketHandler.Send("Hello from ESP32!");
    }
    #endif

    #ifdef I2C_
    if (i2c.IsAvailable())
    {
        if (i2c.GetLastChange() == LAST_ITEM_REGISTER)
        {
            Serial.print("Last address:");
            Serial.println(i2c.GetLastAddress());
            char data[MAX_STRING_SIZE];
            i2c.GetRegister(LAST_ITEM_REGISTER,&data[0]);
            Serial.print("Item: ");
            Serial.println(data);
        }
    }

    if (scanTime < millis())
    {
        i2c.Scan();
        scanTime = millis() + SCAN_OFFSET;
    }
    #endif
}
