// #include <nlohmann/json.hpp>
#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <PlainFileDatabase.hpp>
#include <filesystem>
#include <fstream>


#include "./UserInterface/UserInterface.hpp"

int main()
{
    // Set up database
    FindIt::PlainFileDatabase database("./Data/Database.db");

    // Set up message queues
    FindIt::MessageQueue UIToCommunication;
    FindIt::MessageQueue CommunicationToUI;

    // Set up communication and start a new thread for it
    FindIt::Communication communication(clusterConnection, protocolParser, database, UIToCommunication, CommunicationToUI);
    std::jthread communicationThread2(&FindIt::Communication::Run, &communication);

    FindIt::UserInterface *UI = new FindIt::UserInterface(std::bind_front(&FindIt::PlainFileDatabase::GetAllObjects, &database)
                                                            , std::bind_front(&FindIt::PlainFileDatabase::Add, &database)
                                                            , msgq1
                                                            , msgq2);

    std::jthread t1(&FindIt::UserInterface::Run, UI);
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
    t1.join();

    delete UI;
}
