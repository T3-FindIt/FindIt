#include "NodeRespondToProductRequest.hpp"

namespace FindIt
{

NodeRespondToProductRequest::NodeRespondToProductRequest(std::string product, bool result)
    : product(product), result(result)
{
    this->action = "ResponseProduct";
    this->type = MessageType::NODE_RESPOND_TO_PRODUCT_REQUEST;
}

MessageType NodeRespondToProductRequest::GetType()
{
    return this->type;
}

std::string NodeRespondToProductRequest::GetAction()
{
    return this->action;
}

std::string NodeRespondToProductRequest::GetProduct()
{
    return this->product;
}

bool NodeRespondToProductRequest::GetResult()
{
    return this->result;
}

};
