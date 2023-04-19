#ifndef HEARTBEATRESPONSE_HPP
#define HEARTBEATRESPONSE_HPP

#include "../IMessage.hpp"

namespace FindIt
{

class HeartBeatResponse : public IMessage
{
private:
    std::string node;
    uint32_t places;
public:
    HeartBeatResponse(std::string node, uint32_t places);
    ~HeartBeatResponse() = default;

    MessageType GetType() final;
    std::string GetAction();
    std::string GetNode();
    uint32_t GetPlaces();
};

}

#endif // HEARTBEATRESPONSE_HPP
