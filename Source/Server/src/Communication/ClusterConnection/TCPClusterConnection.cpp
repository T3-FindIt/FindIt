#include "TCPClusterConnection.hpp"

#include <iostream>
#include <sstream>

#pragma comment (lib, "ws2_32.lib")

namespace FindIt
{

void TCPClusterConnection::HandleIncomingMessage(SOCKET &clientSocket)
{
    char buf[4096];
    ZeroMemory(buf, 4096);
    
    // Receive message
    int bytesIn = recv(clientSocket, buf, 4096, 0);
    if (bytesIn <= 0)
    {
        // Drop the client
        closesocket(clientSocket);
        FD_CLR(clientSocket, &this->master);
    }
    else
    {
        std::string inboundMsg = std::string(buf, bytesIn);
        if (this->onMessageHandler != nullptr)
        {
            this->onMessageHandler(inboundMsg);
        }
    }
}

void TCPClusterConnection::Send(SOCKET &clientSocket, std::string msg)
{
    send(clientSocket, msg.c_str(), msg.size() + 1, 0);
}

TCPClusterConnection::TCPClusterConnection(int port, std::string ip)
{
}

void FindIt::TCPClusterConnection::Run()
{
}

void FindIt::TCPClusterConnection::Stop()
{
}

void FindIt::TCPClusterConnection::Broadcast(IMessage& message)
{
}

void FindIt::TCPClusterConnection::SetOnMessageHandler(Event_function_t handler)
{
}

};
