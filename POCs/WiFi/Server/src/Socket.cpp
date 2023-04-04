#include "Socket.hpp"
#include <iostream>
#include <limits.h>

namespace FindIt
{

Socket::Socket(const std::string& ipAddress,
                int port,
                MessageReceivedHandler handler)
        : m_ipAddress(ipAddress),
        m_port(port),
        m_messageReceived(handler)
{
}

void Socket::Send(int clientSocket, const std::string& message)
{
    std::size_t messageLen = message.size() + 1;
    if (messageLen <= INT_MAX)
    {
        send(clientSocket, message.c_str(), (int)messageLen, 0);
    }
}

bool Socket::Init()
{
    WSAData wsaData;
    int wsInit = WSAStartup(MAKEWORD(2, 2), &wsaData);
    //TODO: error handling
    return wsInit == 0;
}

[[noreturn]] void Socket::Run()
{
    char buf[4096];

    while (true)
    {
        SOCKET listening = CreateSocket();
        if (listening == INVALID_SOCKET)
        {
            break;
        }

        SOCKET client = WaitForConnection(listening);
        if (client != INVALID_SOCKET)
        {
            closesocket(listening);

            int bytesReceived = 0;
            do
            {
                ZeroMemory(buf, 4096);
                bytesReceived = recv(client, buf, 4096, 0);
                if (bytesReceived > 0 && m_messageReceived != nullptr)
                {
                    m_messageReceived(this, client, std::string(buf, 0, bytesReceived));
                }
            } while (bytesReceived > 0);
            closesocket(client);
        }
    }
}

Socket::~Socket()
{
    WSACleanup();
}

SOCKET Socket::CreateSocket()
{
    SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening != INVALID_SOCKET)
    {
        sockaddr_in hint;
        hint.sin_family = AF_INET;
        hint.sin_port = htons(m_port);

        hint.sin_addr.S_un.S_addr = inet_addr(m_ipAddress.c_str());
        // hint.sin_addr.S_un.S_addr = INADDR_ANY;

        int bindOk = bind(listening, (sockaddr *) &hint, sizeof(hint));
        if (bindOk != SOCKET_ERROR)
        {
            int listenOk = listen(listening, SOMAXCONN);
            if (listenOk != SOCKET_ERROR)
            {
                return listening;
            }
        }
    }
    return INVALID_SOCKET;
    //return -1;
}

SOCKET Socket::WaitForConnection(SOCKET listening)
{
    SOCKET client = accept(listening, nullptr, nullptr);
    return client;
}

} // namespace FindIt
