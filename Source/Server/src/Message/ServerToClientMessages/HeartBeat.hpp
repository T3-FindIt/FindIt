#ifndef HEARTBEAT_HPP
#define HEARTBEAT_HPP

#include <IMessage.hpp>

namespace FindIt
{

class HeartBeat : public IMessage
{
public:
    HeartBeat();
    ~HeartBeat() = default;

    MessageType GetType() final;
    std::string GetAction();
};

}

#endif // HEARTBEAT_HPP
