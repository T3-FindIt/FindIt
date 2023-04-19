#include "HeartBeat.hpp"

namespace FindIt
{
HeartBeat::HeartBeat()
{
    this->type = MessageType::HEARTBEAT;
}

MessageType HeartBeat::GetType()
{
    return this->type;
}

std::string HeartBeat::GetAction() const
{
    return this->action;
}

};
