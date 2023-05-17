#include "Communication.hpp"

#include <thread>
#include <iostream>
#include <ctime>
#include <chrono>
#include <list>

namespace FindIt
{

struct client_t
{
    uint64_t id;
    std::chrono::time_point<std::chrono::system_clock> lastCommunication;

    std::chrono::time_point<std::chrono::system_clock> last_out_message_time;
    FindIt::MessageType last_out_message;
};

std::list<client_t> clients;

void onMessage(const uint64_t client, const std::string& message)
{
    client_t* subject = nullptr;
    for (auto it = clients.begin(); it != clients.end(); ++it)
    {
        if (it->id == client)
        {
            subject = std::to_address(it);
            break;
        }
    }
    std::string firstPart = message.substr(0, message.find_first_of(','));
    if (firstPart == "{\"action\": \"HeartBeat\""
        && subject != nullptr)
    {
        subject->lastCommunication = std::chrono::system_clock::now();
        subject->last_out_message = MessageType::INVALID;
    }

    std::cout << "Client " << subject->id << " says: " << message << std::endl;
}

void onConnect(const uint64_t client)
{
    clients.push_back((client_t){.id = client,
                                    .lastCommunication = std::chrono::system_clock::now(),
                                    .last_out_message_time = std::chrono::system_clock::now(),
                                    .last_out_message = MessageType::INVALID});
    std::cout << "Client " << client << " connected" << std::endl;
}

void onDisconnect(const uint64_t client)
{
    for (auto it = clients.begin(); it != clients.end(); ++it)
    {
        if (it->id == client)
        {
            clients.erase(it);
            break;
        }
    }
    std::cout << "Client " << client << " disconnected" << std::endl;
}

Communication::Communication(IClusterConnection &clusterConnection, IProtocolParser &protocolParser)
    : clusterConnection(clusterConnection), protocolParser(protocolParser)
{
    this->clusterConnection.setCallbacks(onMessage, onConnect, onDisconnect);
}

Communication::~Communication()
{
    this->Stop();
}

void Communication::Run()
{
    this->isRunning = true;
    IClusterConnection* _clusterConnection = &clusterConnection;
    std::jthread server_thread(&IClusterConnection::run, _clusterConnection);
    while (this->isRunning)
    {
        bool isMessageSent = false;
        for (auto client = clients.begin(); client != clients.end(); ++client)
        {
            if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - client->lastCommunication)
                    > std::chrono::milliseconds(MAX_CLIENT_INACTIVITY_TIME)
                && client->last_out_message != MessageType::HEARTBEAT
                && !isMessageSent)
            {
                std::string msg = "{\"action\": \"HeartBeat\"}\0";
                for (auto it = clients.begin(); it != clients.end(); ++it)
                    it->last_out_message = MessageType::HEARTBEAT;

                this->clusterConnection.broadcastMessage(msg);
                isMessageSent = true;
            }

        }

        if (isMessageSent)
        {
            for (auto client = clients.begin(); client != clients.end(); ++client)
                if (client->last_out_message != MessageType::INVALID)
                {
                    client->last_out_message = MessageType::HEARTBEAT;
                    client->last_out_message_time = std::chrono::system_clock::now();
                }
        }

        for (auto client = clients.begin(); client != clients.end(); ++client)
        {
            if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - client->lastCommunication)
                    > std::chrono::milliseconds((MAX_CLIENT_INACTIVITY_TIME * 2)))
            {
                std::cout << "Disconnecting Client " << client->id << std::endl;
                this->clusterConnection.closeClient(client->id);
                clients.erase(client);
                break;
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Save CPU
    }
    this->clusterConnection.stop();
    server_thread.join();
}

void Communication::Stop()
{
    this->isRunning = false;
}

};
