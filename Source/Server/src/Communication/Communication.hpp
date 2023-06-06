#ifndef COMMUNICATION_HPP
#define COMMUNICATION_HPP

#include <IClusterConnection.hpp>
#include <IProtocolParser.hpp>

#include <IMessage.hpp>
#include <HeartBeat.hpp>
#include <HeartbeatResponse.hpp>
#include <NodeSignIn.hpp>
#include <NodeSignInResponse.hpp>

#include <chrono>

constexpr uint64_t MAX_CLIENT_INACTIVITY_TIME = 10000;

namespace FindIt
{

struct message_track_t
{
    std::chrono::time_point<std::chrono::system_clock> time = std::chrono::system_clock::now();
    FindIt::MessageType type = FindIt::MessageType::INVALID;
    std::shared_ptr<FindIt::IMessage> data = nullptr;
};

struct client_t
{
    uint64_t id;
    std::chrono::time_point<std::chrono::system_clock> lastCommunication = std::chrono::system_clock::now();

    message_track_t lastInMessage;
    message_track_t lastOutMessage;
};

class Communication
{
public:
    Communication(IClusterConnection& clusterConnection, IProtocolParser& protocolParser);
    ~Communication();

    void Run();
    void Stop();
private:
    IClusterConnection& clusterConnection;
    IProtocolParser& protocolParser;
    bool isRunning = false;
    std::unordered_map<uint64_t, client_t> clients;

    void onMessage(const uint64_t client, const std::string& message);
    void onConnect(const uint64_t client);
    void onDisconnect(const uint64_t client);
};

};

#endif // COMMUNICATION_HPP
