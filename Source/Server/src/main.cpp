#include <nlohmann/json.hpp>
#include <iostream>
#include <thread>

#include <Communication.hpp>
#include "./Communication/ClusterConnection/TCPClusterConnection.hpp"
#include "./Communication/ProtocolParser/JSONProtocolParser.hpp"

int main()
{
    FindIt::TCPClusterConnection* clusterConnection = new FindIt::TCPClusterConnection(54000, "192.168.2.133");
    FindIt::JSONProtocolParser* protocolParser = new FindIt::JSONProtocolParser();
    FindIt::Communication* communication = new FindIt::Communication(*clusterConnection, *protocolParser);

    std::cout << "Starting communication thread" << std::endl;

    std::thread communicationThread(&FindIt::Communication::Run, communication);
    communicationThread.detach();

    std::cin.get();

    std::cout << "Stopping communication thread" << std::endl;
    communication->Stop();

    delete communication;
    delete protocolParser;
    delete clusterConnection;

    return 0;
}
