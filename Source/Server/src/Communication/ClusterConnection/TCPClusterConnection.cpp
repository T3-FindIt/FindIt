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
    : port(port), ip(ip)
{
    // Nothing to do here
}

TCPClusterConnection::~TCPClusterConnection()
{
    this->Stop();
}

// Code from (with some modifications):
// https://www.youtube.com/watch?v=dquxuXeZXgo
// https://github.com/codehoose/cpp-networking/blob/main/MultipleClientsBarebonesServer/MultipleClientsBarebonesServer/main.cpp

void TCPClusterConnection::Run()
{

    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);

    // Initialize winsock
    int wsOk = WSAStartup(ver, &wsData);
    if (wsOk != 0)
    {
        std::cerr << "Winsock failed to initialize. Quitting." << std::endl;
        return;
    }
    
    // Create a socket
    this->listeningSock = socket(AF_INET, SOCK_STREAM, 0);
    if (this->listeningSock == INVALID_SOCKET)
    {
        std::cerr << "Failed to create listening socket. Quitting." << std::endl;
        return;
    }

    this->isRunning = true;

    // Bind the ip address and port to a socket
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(this->port);
    hint.sin_addr.S_un.S_addr = INADDR_ANY;
    
    bind(this->listeningSock, (sockaddr*)&hint, sizeof(hint));

    // Tell Winsock the socket is for listening 
    listen(this->listeningSock, SOMAXCONN);

    // Create the master file descriptor set and zero it
    FD_ZERO(&this->master);

    // Create the listening socket and add it to the master set
    FD_SET(this->listeningSock, &this->master);

    while (this->isRunning)
    {
        fd_set copy = this->master;

        // Get the number of sockets that are ready to be read
        int socketCount = select(0, &copy, nullptr, nullptr, nullptr);

        // Loop through all the current connections / potential connect
        for (int i = 0; i < socketCount; i++)
        {
            // Get the current socket
            SOCKET sock = copy.fd_array[i];

            // Is it an potential new connection?
            if (sock == this->listeningSock)
{
                // Accept a new connection
                SOCKET client = accept(this->listeningSock, nullptr, nullptr);

                // Add the new connection to the list of connected clients
                FD_SET(client, &this->master);
            }
            // Handle incoming message
            else
            {
                this->HandleIncomingMessage(sock);
            }

            // Broadcast message if there is one
            if (this->broadcast && this->broadcastMsg != nullptr)
            {
                this->Send(sock, *this->broadcastMsg);
}
        }

        // Clear broadcast message
        if (this->broadcast && this->broadcastMsg != nullptr)
        {
            this->broadcast = false;
            this->broadcastMsg = nullptr;
        }
    }

    // Remove the listening socket from the master file descriptor set and close it
    // to prevent anyone else trying to connect.
    FD_CLR(this->listeningSock, &this->master);
    closesocket(this->listeningSock);

    // Message to let users know what's happening.
    // std::string msg = "Server is shutting down.\r\n";

    while (this->master.fd_count > 0)
{
        // Get the socket number
        SOCKET sock = this->master.fd_array[0];

        // Send the goodbye message
        // this->Send(sock, msg);

        // Remove it from the master file list and close the socket
        FD_CLR(sock, &this->master);
        closesocket(sock);
}

    WSACleanup();
}

{
}

void FindIt::TCPClusterConnection::Broadcast(IMessage& message)
{
}

void FindIt::TCPClusterConnection::SetOnMessageHandler(Event_function_t handler)
{
}

};
