#include "WiFiHandler.hpp"

#define ATTEMPTS 10
#define DELAY_BETWEEN_ATTEMPTS 1000

WiFiHandler::WiFiHandler()
{
}

WiFiHandler::WiFiHandler(WiFiData &data)
{
    if (data.GetSSID() == "" || data.GetPassword() == "")
    {
        throw "Invalid WiFiData";
    }
    this->data = data;
}

bool WiFiHandler::isConnected()
{
    return WiFi.isConnected();
}

void WiFiHandler::Connect()
{
    WiFi.begin(data.GetSSID().c_str(), data.GetPassword().c_str());
    for (int i = 0; i < ATTEMPTS; i++)
    {
        if (WiFi.isConnected())
        {
            break;
        }
        delay(DELAY_BETWEEN_ATTEMPTS);
    }

    if (WiFi.isConnected())
    {
        Serial.println("Connected to WiFi");
    }
    else
    {
        Serial.println("Failed to connect to WiFi");
    }
}

void WiFiHandler::Disconnect()
{
    WiFi.disconnect();
}
