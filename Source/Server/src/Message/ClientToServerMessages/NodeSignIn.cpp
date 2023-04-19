#include "NodeSignIn.hpp"
namespace FindIt
{

NodeSignIn::NodeSignIn(std::string node, uint32_t places)
    : node(node), places(places)
{
    this->action = "SignIn";
    this->type = MessageType::NODE_SIGN_IN;
}

MessageType NodeSignIn::GetType()
{
    return this->type;
}

std::string NodeSignIn::GetAction()
{
    return this->action;
}

std::string NodeSignIn::GetNode()
{
    return this->node;
}

uint32_t NodeSignIn::GetPlaces()
{
    return this->places;
}

};