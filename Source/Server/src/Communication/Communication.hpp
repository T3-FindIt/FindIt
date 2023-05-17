#ifndef COMMUNICATION_HPP
#define COMMUNICATION_HPP

#include <IClusterConnection.hpp>
#include <IProtocolParser.hpp>

#include <IMessage.hpp>
#include <HeartBeat.hpp>
#include <HeartbeatResponse.hpp>

#define MAX_CLIENT_INACTIVITY_TIME 10000

namespace FindIt
{

class Communication
{
private:
    IClusterConnection& clusterConnection;
    IProtocolParser& protocolParser;
    bool isRunning = false;
public:
    Communication(IClusterConnection& clusterConnection, IProtocolParser& protocolParser);
    ~Communication();

    void Run();
    void Stop();
};

};

#endif // COMMUNICATION_HPP
