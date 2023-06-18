#include "Communication.hpp"

#include <thread>
#include <iostream>
#include <ctime>
#include <chrono>
#include <list>

namespace FindIt
{

Communication::Communication(IClusterConnection &clusterConnection
                            , IProtocolParser &protocolParser
                            , IDatabase &database
                            , MessageQueue &queueIn
                            , MessageQueue &queueOut)
    : clusterConnection(clusterConnection)
    , protocolParser(protocolParser)
    , database(database)
    , queueIn(queueIn)
    , queueOut(queueOut)
{
    clusterConnection.setCallbacks(std::bind_front(&Communication::onMessage, this),
                                    std::bind_front(&Communication::onConnect, this),
                                    std::bind_front(&Communication::onDisconnect, this));
}

Communication::~Communication()
{
    Stop();
}

void Communication::Run()
{
    isRunning = true;
    std::jthread server_thread(&IClusterConnection::run, &clusterConnection);
    while (isRunning)
    {
        // bool isMessageSent = false;
        // for (auto client = clients.begin(); client != clients.end(); ++client)
        // {
        //     if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - client->lastCommunication)
        //             > std::chrono::milliseconds(MAX_CLIENT_INACTIVITY_TIME)
        //         && client->last_out_message != MessageType::HEARTBEAT
        //         && !isMessageSent)
        //     {
        //         std::string msg = "{\"action\": \"HeartBeat\"}\0";
        //         for (auto it = clients.begin(); it != clients.end(); ++it)
        //             it->last_out_message = MessageType::HEARTBEAT;

        //         clusterConnection.broadcastMessage(msg);
        //         isMessageSent = true;
        //     }

        // }

        // if (isMessageSent)
        // {
        //     for (auto client = clients.begin(); client != clients.end(); ++client)
        //         if (client->last_out_message != MessageType::INVALID)
        //         {
        //             client->last_out_message = MessageType::HEARTBEAT;
        //             client->last_out_message_time = std::chrono::system_clock::now();
        //         }
        // }

        for (auto& [ID, obj] : clients)
        {
            if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - obj.lastCommunication)
                    > std::chrono::milliseconds((MAX_CLIENT_INACTIVITY_TIME * 2)))
            {
                std::cout << "Disconnecting Client " << ID << std::endl;
                clusterConnection.closeClient(ID);
                clients.erase(ID);
                break;
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Save CPU
    }
    clusterConnection.stop();
    server_thread.join();
}

void Communication::Stop()
{
    isRunning = false;
}

void Communication::onMessage(const uint64_t client, const std::string &message)
{
    client_t* subject = nullptr;
    
    if (clients.contains(client))
    {
        subject = &clients.at(client);
    }
    else
    {
        return;
    }
    
    std::shared_ptr<FindIt::IMessage> msg = protocolParser.Parse(message);
    
    if (msg == nullptr)
    {
        return;
    }

    auto setLastInMessage = [subject](MessageType type, std::shared_ptr<FindIt::IMessage> msg)
    {
        subject->lastCommunication = std::chrono::system_clock::now();
        subject->lastInMessage.type = type;
        subject->lastInMessage.time = std::chrono::system_clock::now();
        subject->lastInMessage.data = msg;
    };

    auto setLastOutMessage = [subject](MessageType type, std::shared_ptr<FindIt::IMessage> msg)
    {
        subject->lastCommunication = std::chrono::system_clock::now();
        subject->lastOutMessage.type = type;
        subject->lastOutMessage.time = std::chrono::system_clock::now();
        subject->lastOutMessage.data = msg;
    };

    setLastInMessage(msg->GetType(), msg);

    if (subject->lastInMessage.type == MessageType::NODE_SIGN_IN)
    {
        auto signInMsg = std::dynamic_pointer_cast<NodeSignIn>(msg);
        std::shared_ptr<FindIt::NodeSignInResponse> response;
        if (clients.contains(client))
        {
            response = std::make_shared<FindIt::NodeSignInResponse>(signInMsg->GetNode(), signInMsg->GetPlaces(), false);
        }
        else
        {
            response = std::make_shared<FindIt::NodeSignInResponse>(signInMsg->GetNode(), signInMsg->GetPlaces(), true);
        }
        clients.try_emplace(client, client_t{.id = client, .lastCommunication = std::chrono::system_clock::now()});
        clusterConnection.sendMessage(client, protocolParser.Parse(*response));
        setLastOutMessage(response->GetType(), response);
    }
    else if (subject->lastInMessage.type == MessageType::NODE_NOTIFY_NEW_PRODUCT)
    {
        auto notifyMsg = std::dynamic_pointer_cast<NodeNotifyNewProduct>(msg);
        std::shared_ptr<FindIt::NodeNotifyNewProductResponse> response;
        ItemType type(notifyMsg->GetProduct());
        database.Add(type);
    }
}

void Communication::onConnect(const uint64_t client)
{
    std::cout << "Client " << client << " connected" << std::endl;
}

void Communication::onDisconnect(const uint64_t client)
{
    clients.erase(client);
    std::cout << "Client " << client << " disconnected" << std::endl;
}
};
