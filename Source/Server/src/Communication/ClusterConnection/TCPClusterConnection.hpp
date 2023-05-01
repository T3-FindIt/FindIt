#ifndef TCPCLUSTERCONNECTION_HPP
#define TCPCLUSTERCONNECTION_HPP

#include "IClusterConnection.hpp"

#include <string>
#include <WS2tcpip.h>

namespace FindIt
{

class TCPClusterConnection : public IClusterConnection
{
private:
    int port;
    std::string ip;

    fd_set master;
    SOCKET listeningSock;
    bool broadcast = false;
    std::string* broadcastMsg = nullptr;

    void HandleIncomingMessage(SOCKET& clientSocket);
    void Send(SOCKET& clientSocket, std::string msg);
public:
    TCPClusterConnection(int port, std::string ip);
    ~TCPClusterConnection();
    void Run() final;
    void Stop() final;
    void Broadcast(std::string& message) final;
    void SetOnMessageHandler(Event_function_t handler) final;
};

};

#endif //ICLUSTERCONNECTION_HPP
