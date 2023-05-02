#include "Communication.hpp"

#include <thread>
#include <iostream>

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
    std::cout << "Entering communication loop" << std::endl;
    while (this->isRunning)
    {
        std::cout << "Sending heartbeat" << std::endl;
        std::string msg = "{\"action\": \"HeartBeat\"}\r\n\0";
        this->clusterConnection.Broadcast(msg);
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    std::cout << "Exiting communication loop" << std::endl;
    std::cout << "Stopping TCP thread" << std::endl;
    this->clusterConnection.Stop();
    clusterConnectionThread.join();
    std::cout << "TCP thread stopped" << std::endl;
}

void Communication::Stop()
{
    this->isRunning = false;
}

};
