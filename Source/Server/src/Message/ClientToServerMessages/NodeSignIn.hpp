#ifndef NODESIGNIN_HPP
#define NODESIGNIN_HPP

#include "../IMessage.hpp"

namespace FindIt
{

class NodeSignIn : public IMessage
{
private:
    std::string node;
    uint32_t places;
public:
    NodeSignIn(std::string node, uint32_t places);
    ~NodeSignIn() = default;

    MessageType GetType() final;
    std::string GetAction();
    std::string GetNode();
    uint32_t GetPlaces();
};

}

#endif // NODESIGNIN_HPP
