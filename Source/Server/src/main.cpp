#include <iostream>
#include <thread>
#include <vector>
#include <memory>

#include "Communication.hpp"
#include "TCPConnection.hpp"
#include "JSONProtocolParser.hpp"
#include "ItemType.hpp"
#include "PlainFileDatabase.hpp"
#include "IMessage.hpp"
#include "MessageQueue.hpp"
#include "UserInterface.hpp"

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
    std::jthread communicationThread(&FindIt::Communication::Run, &communication);

    // Set up UI and start a new thread for it
    FindIt::UserInterface UI(std::bind_front(&FindIt::PlainFileDatabase::GetAllObjects, &database)
                                                , std::bind_front(&FindIt::PlainFileDatabase::Add, &database)
                                                , CommunicationToUI
                                                , UIToCommunication);
    std::jthread uiThread(&FindIt::UserInterface::Run, &UI);

    // Make sure we exit gracefully
    uiThread.join();
    communication.Stop();
    communicationThread.join();
    return 0;
}
