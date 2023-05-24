#include "HeartbeatResponse.hpp"

namespace FindIt
{

HeartBeatResponse::HeartBeatResponse(std::string node, uint32_t places)
    : node(node), places(places)
{
    this->action = "HeartBeat";
    this->type = MessageType::HEARTBEAT_RESPONSE;
}

MessageType HeartBeatResponse::GetType()
{
    return this->type;
}

std::string HeartBeatResponse::GetAction()
{
    return this->action;
}

std::string HeartBeatResponse::GetNode()
{
    return this->node;
}

uint32_t HeartBeatResponse::GetPlaces()
{
    return this->places;
}

};
