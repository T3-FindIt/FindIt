#include "HeartBeat.hpp"

namespace FindIt
{
HeartBeat::HeartBeat()
{
    this->type = MessageType::HEARTBEAT;
    this->action = "HeartBeat";
}

MessageType HeartBeat::GetType()
{
    return this->type;
}

std::string HeartBeat::GetAction()
{
    return this->action;
}

};
