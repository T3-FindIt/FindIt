#include <Arduino.h>

#include "I2C.hpp"
#include "WiFiHandler.hpp"
#include "WebSocketHandler.hpp"

#define LOCAL_ADDRESS 0x1

// Might be better to put this in a secret.h file
#define SSID "SSID"
#define PASSWORD "PASSWORD"

#define SERVER_ADDRESS "SERVER IP"
#define SERVER_PORT 80

void setup() {
  WiFiData wifiData(SSID, PASSWORD);
  WiFiHandler wifiHandler(wifiData);

  WebSocketData webSocketData(SERVER_ADDRESS, SERVER_PORT);
  WebSocketHandler webSocketHandler(webSocketData);

  wifiHandler.Connect();
  if(!wifiHandler.isConnected())
  {
    return;
  }

  webSocketHandler.Connect();
  if(!webSocketHandler.isConnected())
  {
    return;
  }

  I2C i2c(0x1);
}

void loop() {
  // put your main code here, to run repeatedly:
}
