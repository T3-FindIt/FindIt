#include "Communication.hpp"

#include <thread>
#include <iostream>
#include <ctime>    

namespace FindIt
{

Communication::Communication(IClusterConnection &clusterConnection, IProtocolParser &protocolParser)
    : clusterConnection(clusterConnection), protocolParser(protocolParser)
{
}

Communication::~Communication()
{
    this->Stop();
}

void Communication::Run()
{
    this->isRunning = true;
    std::cout << "Starting TCP thread" << std::endl;
    std::thread clusterConnectionThread(&IClusterConnection::Run, &this->clusterConnection);
    clusterConnectionThread.detach();
    std::cout << "Entering communication loop" << std::endl;
    while (this->isRunning)
    {
        std::cout << "Sending heartbeat" << std::endl;
        std::string msg = "{\"action\": \"HeartBeat\"}\r\n\0";
        this->clusterConnection.Broadcast(msg);
        auto start = std::chrono::system_clock::now();
        auto end = std::chrono::system_clock::now();
        while ((end - start) < std::chrono::seconds(5) && this->isRunning)
        {
            end = std::chrono::system_clock::now();
        }
    }
    std::cout << "Exiting communication loop" << std::endl;
    std::cout << "Stopping TCP thread" << std::endl;
    this->clusterConnection.Stop();
    std::cout << "TCP thread stopped" << std::endl;
}

void Communication::Stop()
{
    this->isRunning = false;
}

};
