#ifndef NODESIGNINRESPONSE_HPP
#define NODESIGNINRESPONSE_HPP

#include "../IMessage.hpp"

namespace FindIt
{

class NodeSignInResponse : public IMessage
{
private:
    std::string node;
    uint32_t places;
    bool result;
public:
    NodeSignInResponse(std::string node, uint32_t places, bool result);
    ~NodeSignInResponse() = default;

    MessageType GetType() final;
    std::string GetAction();
    std::string GetNode();
    uint32_t GetPlaces();
    bool GetResult();
};

}

#endif // NODESIGNINRESPONSE_HPP
