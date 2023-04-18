#include "WebSocketHandler.hpp"
#include <WiFi.h>

WiFiClient client;

WebSocketHandler::WebSocketHandler()
{
}

WebSocketHandler::WebSocketHandler(WebSocketData &data)
{
    if(data.GetIP() == "" || data.GetPort() == 0)
    {
        throw "Invalid WebSocketData";
    }
    this->data = data;
}

void WebSocketHandler::Send(std::string json)
{
    if (isConnected())
    {
        client.print(json.c_str());
    }
}

std::string WebSocketHandler::Recieve()
{
    if (isConnected())
    {
        if (client.available())
        {
            return client.readStringUntil('\n').c_str();
        }
    }
    return "";
}

bool WebSocketHandler::Connect()
{
    if (!isConnected())
    {
        client.connect(data.GetIP().c_str(), data.GetPort());
    }
    return isConnected();
}

void WebSocketHandler::Disconnect()
{
    if (isConnected())
    {
        client.stop();
    }
}

bool WebSocketHandler::isConnected()
{
    return client.connected();
}
