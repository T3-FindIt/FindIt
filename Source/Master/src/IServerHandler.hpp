#ifndef ISERVERHANDLER_HPP
#define ISERVERHANDLER_HPP

#include <string>

class IServerHandler
{
    public:
    virtual void Send(std::string json) = 0;
    virtual std::string Recieve() = 0;

    virtual bool Connect() = 0;
    virtual void Disconnect() = 0;
    virtual bool isConnected() = 0;
};

#endif // ISERVERHANDLER_HPP
