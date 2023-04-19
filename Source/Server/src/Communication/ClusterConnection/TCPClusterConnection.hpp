#ifndef TCPCLUSTERCONNECTION_HPP
#define TCPCLUSTERCONNECTION_HPP

#include "IClusterConnection.hpp"

#include <string>

namespace FindIt
{

class TCPClusterConnection : public IClusterConnection
{
private:
    int port;
    std::string ip;
public:
    TCPClusterConnection(int port, std::string ip);
    [[noreturn]] void Run() final;
    void Stop() final;
    void Broadcast(IMessage& message) final;
    void SetOnMessageHandler(Event_function_t handler) final;
};

};

#endif //ICLUSTERCONNECTION_HPP
