#include <iostream>
#include <thread>
#include <vector>
#include <memory>

#include <Communication.hpp>
#include <TCPConnection.hpp>
#include <JSONProtocolParser.hpp>
#include <ItemType.hpp>
#include <PlainFileDatabase.hpp>
#include <IMessage.hpp>
#include <MessageQueue.hpp>

int main()
{
    // Set cluster coms to a TCP connection on port 54000
    FindIt::Server clusterConnection(54000);

    // Set protocol parser to JSON
    FindIt::JSONProtocolParser protocolParser;

    // Set up communication and start a new thread for it
    FindIt::Communication communication(clusterConnection, protocolParser);
    std::jthread communicationThread(&FindIt::Communication::Run, communication);

    // std::cin.get();

    // communication->Stop();
    // communicationThread.join();


    FindIt::PlainFileDatabase database("./Data/Database.txt");
    database.Add(FindIt::ItemType("TEST_JOHN", 51));
    database.Add(FindIt::ItemType("TEST_TEST", 35));
    database.Add(FindIt::ItemType("TEST_TEST", 35));
    database.Add(FindIt::ItemType("TEST_BARB", 50));
    database.Add(FindIt::ItemType("TEST_JOHNS", 52));
    database.SearchIfPresent(FindIt::ItemType("TEST_TEST", 35));

    std::vector<FindIt::ItemType> objects = database.GetAllObjects();

    database.Remove(FindIt::ItemType("TEST_TEST", 35));

    return 0;
}
