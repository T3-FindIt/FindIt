#include "WiFiHandler.hpp"

WiFiHandler::WiFiHandler(WiFiData& data)
{
    this->data = data;
}

bool WiFiHandler::isConnected()
{
    return WiFi.isConnected();
}

void WiFiHandler::Connect()
{
    WiFi.begin(data.GetSSID().c_str(), data.GetPassword().c_str());
    for (int i = 0; i < 10; i++)
    {
        if (WiFi.isConnected())
        {
            break;
        }
        delay(1000);
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
