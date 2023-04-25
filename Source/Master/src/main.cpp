#include <Arduino.h>

#include "I2C.hpp"
// #include "WiFiHandler.hpp"
// #include "WebSocketHandler.hpp"

#define LOCAL_ADDRESS 0x08

// Might be better to put this in a secret.h file
#define SSID "SSID"
#define PASSWORD "PASSWORD"

#define SERVER_ADDRESS "SERVER IP"
#define SERVER_PORT 80

// WiFiHandler wifiHandler = WiFiHandler();
// WebSocketHandler webSocketHandler = WebSocketHandler();
I2C i2c;

void setup() {
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

  // Serial.begin(9600);
  // Serial.println("Starting up!");
  i2c = I2C(LOCAL_ADDRESS);
}

Node_Registers lastRequest = NR_None;

void loop()
{
  // Turn this into a function?
  // if(webSocketHandler.isConnected())
  // {
  //   std::string websocketData = webSocketHandler.Recieve();
  //   if(websocketData != "")
  //   {
  //     // Decompile with JSON parser
  //     // Do some stuff with the data

  //     Serial.print("Websocket data: ");
  //     Serial.println(websocketData.c_str());
  //   }
  // }

  // i2c.Send(0x7F, NR_Notification, 0x5);
  // delay(1000);
}
