#include "NodeEventProductFound.hpp"

namespace FindIt
{

NodeEventProductFound::NodeEventProductFound(std::string product, bool result)
    : product(product), result(result)
{
    this->action = "NodeEventProductFound";
    this->type = MessageType::NODE_EVENT_PRODUCT_FOUND;
}

MessageType NodeEventProductFound::GetType()
{
    return this->type;
}

std::string NodeEventProductFound::GetAction()
{
    return this->action;
}

std::string NodeEventProductFound::GetProduct()
{
    return this->product;
}

bool NodeEventProductFound::GetResult()
{
    return this->result;
}

};
