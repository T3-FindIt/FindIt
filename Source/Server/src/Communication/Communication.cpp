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

        // if (isMessageSent)
        // {
        //     for (auto client = clients.begin(); client != clients.end(); ++client)
        //         if (client->last_out_message != MessageType::INVALID)
        //         {
        //             client->last_out_message = MessageType::HEARTBEAT;
        //             client->last_out_message_time = std::chrono::system_clock::now();
        //         }
        // }

        if (queueIn.returnSize() > 0)
        {
            // std::cout << "Sending message to all clients" << std::endl;
            std::shared_ptr<FindIt::IMessage> msg = queueIn.pop();
            clusterConnection.broadcastMessage(protocolParser.Parse(*msg));
            for (auto& [ID, obj] : clients)
            {
                obj.lastOutMessage.type = msg->GetType();
                obj.lastOutMessage.time = std::chrono::system_clock::now();
            }
        }

        for (auto& [ID, obj] : clients)
        {
            // Forcefully disconnect client if it has been inactive for too long
            if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - obj.lastCommunication)
                    > std::chrono::milliseconds((MAX_CLIENT_INACTIVITY_TIME * 2)))
            {
                // std::cout << "Disconnecting Client " << ID << std::endl;
                clusterConnection.closeClient(ID);
                clients.erase(ID);
            }

            // Send heartbeat if client has been inactive for too long
            if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - obj.lastCommunication)
                    > std::chrono::milliseconds(MAX_CLIENT_INACTIVITY_TIME)
                && obj.lastOutMessage.type != MessageType::HEARTBEAT)
            {
                // std::cout << "Sending heartbeat to Client " << ID << std::endl;
                FindIt::HeartBeat msgObj;
                std::string msg = protocolParser.Parse(msgObj);
                clusterConnection.sendMessage(ID, msg);
                obj.lastOutMessage.type = MessageType::HEARTBEAT;
                obj.lastOutMessage.time = std::chrono::system_clock::now();

                obj.lastCommunication = std::chrono::system_clock::now();
                obj.lastOutMessage.type = MessageType::INVALID;
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
    std::shared_ptr<FindIt::IMessage> msg = protocolParser.Parse(message);

    // Invalid message
    if (msg == nullptr)
    {
        return;
    }

    // clients.at(client).lastCommunication = std::chrono::system_clock::now();

    auto setLastInMessage = [this](uint64_t client, MessageType type, std::shared_ptr<FindIt::IMessage> msg)
    {
        clients.at(client).lastCommunication = std::chrono::system_clock::now();
        clients.at(client).lastInMessage.type = type;
        clients.at(client).lastInMessage.time = std::chrono::system_clock::now();
        clients.at(client).lastInMessage.data = msg;
    };

    auto setLastOutMessage = [this](uint64_t client, MessageType type, std::shared_ptr<FindIt::IMessage> msg)
    {
        clients.at(client).lastCommunication = std::chrono::system_clock::now();
        clients.at(client).lastOutMessage.type = type;
        clients.at(client).lastOutMessage.time = std::chrono::system_clock::now();
        clients.at(client).lastOutMessage.data = msg;
    };

    if (!clients.contains(client) && msg->GetType() == MessageType::NODE_SIGN_IN)
    {
        auto signInMsg = std::dynamic_pointer_cast<NodeSignIn>(msg);
        std::shared_ptr<FindIt::NodeSignInResponse> response;
        if (clients.contains(client))
        {
            // std::cout << "Client " << client << " already signed in" << std::endl;
            response = std::make_shared<FindIt::NodeSignInResponse>(signInMsg->GetNode(), signInMsg->GetPlaces(), false);
        }
        else
        {
            // std::cout << "Client " << client << " signed in" << std::endl;
            response = std::make_shared<FindIt::NodeSignInResponse>(signInMsg->GetNode(), signInMsg->GetPlaces(), true);
        }
        clients.try_emplace(client, client_t{.id = client, .lastCommunication = std::chrono::system_clock::now()});
        clusterConnection.sendMessage(client, protocolParser.Parse(*response));
        setLastOutMessage(client, response->GetType(), response);
    }

    setLastInMessage(client, msg->GetType(), msg);

    if (clients.at(client).lastInMessage.type == MessageType::NODE_NOTIFY_NEW_PRODUCT)
    {
        auto notifyMsg = std::dynamic_pointer_cast<NodeNotifyNewProduct>(msg);
        std::shared_ptr<FindIt::NodeNotifyNewProductResponse> response = std::make_shared<FindIt::NodeNotifyNewProductResponse>(notifyMsg->GetProduct(), true);
        clusterConnection.sendMessage(client, protocolParser.Parse(*response));
        setLastOutMessage(client, response->GetType(), response);
        ItemType type(notifyMsg->GetProduct());
        database.Add(type);
    }
    else if (clients.at(client).lastInMessage.type == MessageType::HEARTBEAT_RESPONSE)
    {
        clients.at(client).lastOutMessage.type = MessageType::INVALID;
    }
}

void Communication::onConnect(const uint64_t client)
{
    // std::cout << "Client " << client << " connected" << std::endl;
}

void Communication::onDisconnect(const uint64_t client)
{
    clients.erase(client);
    // std::cout << "Client " << client << " disconnected" << std::endl;
}
};
