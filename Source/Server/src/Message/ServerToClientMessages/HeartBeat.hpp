#ifndef HEARTBEAT_HPP
#define HEARTBEAT_HPP

#include "../IMessage.hpp"

#include <string>

namespace FindIt
{

class HeartBeat : public IMessage
{
private:
    const std::string action = "HeartBeat";
public:
    HeartBeat();
    ~HeartBeat() = default;

    MessageType GetType() final;
    std::string GetAction() const;
};

}

#endif // HEARTBEAT_HPP
