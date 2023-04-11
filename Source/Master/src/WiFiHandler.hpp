#ifndef WIFIHANDLER_HPP
#define WIFIHANDLER_HPP

#include <string>
#include <WiFi.h>

struct WiFiData
{
    private:
    std::string SSID;
    std::string Password;

    public:
    WiFiData()
    {
        SSID = "";
        Password = "";
    }

    WiFiData(std::string ssid, std::string password)
    {
        SSID = ssid;
        Password = password;
    }

    std::string GetSSID()
    {
        return SSID;
    }

    std::string GetPassword()
    {
        return Password;
    }
};

class WiFiHandler
{
    private:
    WiFiData data;
    public:
    WiFiHandler(WiFiData& data);
    bool isConnected();
    void Connect();
    void Disconnect();
};

#endif
