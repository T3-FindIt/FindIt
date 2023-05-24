#include <iostream>
#include <thread>
#include <vector>

#include <Communication.hpp>
#include <TCPConnection.hpp>
#include <JSONProtocolParser.hpp>
#include <Object.hpp>
#include <PlainFileDatabase.hpp>


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
    database.Add(FindIt::Object("TEST_JOHN", 51));
    database.Add(FindIt::Object("TEST_TEST", 35));
    database.Add(FindIt::Object("TEST_TEST", 35));
    database.Add(FindIt::Object("TEST_BARB", 50));
    database.Add(FindIt::Object("TEST_JOHNS", 52));
    database.SearchIfPresent(FindIt::Object("TEST_TEST", 35));

    std::vector<FindIt::Object> objects = database.GetAllObjects();

    database.Remove(FindIt::Object("TEST_TEST", 35));

    return 0;
}
