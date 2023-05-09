#include <nlohmann/json.hpp>
#include <iostream>
#include <thread>

#include <Communication.hpp>
#include "./Communication/ClusterConnection/TCPConnection.hpp"
#include "./Communication/ProtocolParser/JSONProtocolParser.hpp"

int main()
{
    // Set cluster coms to a TCP connection on port 54000
    FindIt::Server* clusterConnection = new FindIt::Server(54000);

    // Set protocol parser to JSON
    FindIt::JSONProtocolParser* protocolParser = new FindIt::JSONProtocolParser();

    // Set up communication and start a new thread for it
    FindIt::Communication* communication = new FindIt::Communication(*clusterConnection, *protocolParser);
    std::jthread communicationThread(&FindIt::Communication::Run, communication);

    std::cin.get();

    communication->Stop();
    communicationThread.join();

    delete communication;
    delete protocolParser;
    delete clusterConnection;

    return 0;
}
