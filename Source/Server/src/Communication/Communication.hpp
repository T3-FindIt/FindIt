#ifndef COMMUNICATION_HPP
#define COMMUNICATION_HPP

#include "./ClusterConnection/IClusterConnection.hpp"
#include "./ProtocolParser/IProtocolParser.hpp"

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
