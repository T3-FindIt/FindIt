#ifndef WEBSOCKETHANDLER_HPP
#define WEBSOCKETHANDLER_HPP

#include <string>
#include "IServerHandler.hpp"

struct WebSocketData
{
    std::string IP;
    int Port;

    WebSocketData(std::string ip, int port)
    {
        IP = ip;
        Port = port;
    }

    std::string GetIP()
    {
        return IP;
    }

    int GetPort()
    {
        return Port;

    }
};

class WebSocketHandler : public IServerHandler
{
    private:
    public:
    WebSocketHandler(int port);
    void Send(std::string json);
    std::string Recieve();

    bool Connect();
    void Disconnect();
    bool isConnected();
};

#endif
