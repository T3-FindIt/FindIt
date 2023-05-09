#include "NodeSignInResponse.hpp"

namespace FindIt
{

FindIt::NodeSignInResponse::NodeSignInResponse(std::string node, uint32_t places, bool result)
    : node(node), places(places), result(result)
{
    this->action = "SignIn";
    this->type = MessageType::NODE_SIGN_IN_RESPONSE;
}

MessageType NodeSignInResponse::GetType()
{
    return this->type;
}

std::string NodeSignInResponse::GetAction()
{
    return this->action;
}

std::string NodeSignInResponse::GetNode()
{
    return this->node;
}

uint32_t NodeSignInResponse::GetPlaces()
{
    return this->places;
}

bool NodeSignInResponse::GetResult()
{
    return this->result;
}

};
