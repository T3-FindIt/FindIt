#include <Arduino.h>

#include "I2C.hpp"
#include "WiFiHandler.hpp"
#include "WebSocketHandler.hpp"

#define LOCAL_ADDRESS 0x8 // slave address

// Might be better to put this in a secret.h file. To be discussed with the Git Master
#define SSID "SSID"
#define PASSWORD "PASSWORD"

#define SERVER_ADDRESS "SERVER IP"
#define SERVER_PORT 80

WiFiHandler wifiHandler = WiFiHandler();
WebSocketHandler webSocketHandler = WebSocketHandler();
I2C i2c (LOCAL_ADDRESS);

void setup()
{
    // WiFiData wifiData(SSID, PASSWORD);
    // wifiHandler = WiFiHandler(wifiData);

    // WebSocketData webSocketData(SERVER_ADDRESS, SERVER_PORT);
    // webSocketHandler = WebSocketHandler(webSocketData);

    // wifiHandler.Connect();
    // if(!wifiHandler.isConnected())
    // {
    //   return;
    // }

    // webSocketHandler.Connect();
    // if(!webSocketHandler.isConnected())
    // {
    //   return;
    // }
    Serial.begin(9600);
    Serial.println();
    Serial.println("Starting up!");
}

Node_Registers lastRequest = NR_None;

void loop()
{
    // if(webSocketHandler.isConnected())
    // {
    //   std::string websocketData = webSocketHandler.Recieve();
    //   if(websocketData != "")
    //   {
    //     // Decompile with JSON parser
    //     // Do some stuff with the data

    //     Serial.println(websocketData.c_str());
    //   }

    //   webSocketHandler.Send("Hello from ESP32!");
    // }

    Node_Registers lastRegister = i2c.GetLastChange();
    char data[MAX_STRING_SIZE];

    if (lastRegister != Node_Registers::NR_None)
    {
        i2c.GetRegister(lastRegister, data);
        Serial.print("Register: ");
        Serial.println((int)lastRegister);
        Serial.print("Data: ");
        Serial.println(data);
    }
}
