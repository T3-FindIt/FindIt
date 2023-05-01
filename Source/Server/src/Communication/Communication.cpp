#include "Communication.hpp"

namespace FindIt
{

Communication::Communication(IClusterConnection &clusterConnection, IProtocolParser &protocolParser)
    : clusterConnection(clusterConnection), protocolParser(protocolParser)
{
}

Communication::~Communication()
{
    this->Stop();
}

void Communication::Run()
{
    // TODO: Implement this method
}

void Communication::Stop()
{
    this->isRunning = false;
}

};
