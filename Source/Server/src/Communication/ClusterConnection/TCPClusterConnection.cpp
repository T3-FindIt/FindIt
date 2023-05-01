#include "TCPClusterConnection.hpp"

#include <iostream>
#include <sstream>

#pragma comment (lib, "ws2_32.lib")

namespace FindIt
{

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
