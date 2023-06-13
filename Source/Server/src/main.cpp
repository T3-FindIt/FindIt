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

    // Set up database
    FindIt::PlainFileDatabase database("./Data/Database.db");

    // Set up message queues
    FindIt::MessageQueue UIToCommunication;
    FindIt::MessageQueue CommunicationToUI;

    // Set up communication and start a new thread for it
    FindIt::Communication communication(clusterConnection, protocolParser, database, UIToCommunication, CommunicationToUI);
    std::jthread communicationThread2(&FindIt::Communication::Run, &communication);

    // std::cin.get();

    // communication->Stop();
    // communicationThread.join();

    database.Add(FindIt::ItemType("TEST_JOHN"));
    database.Add(FindIt::ItemType("TEST_TEST"));
    database.Add(FindIt::ItemType("TEST_TEST"));
    database.Add(FindIt::ItemType("TEST_BARB"));
    database.Add(FindIt::ItemType("TEST_JOHNS"));
    database.SearchIfPresent(FindIt::ItemType("TEST_TEST"));

    std::vector<FindIt::ItemType> objects = database.GetAllObjects();

    database.Remove(FindIt::ItemType("TEST_TEST"));
    communication.Stop();

    return 0;
}
