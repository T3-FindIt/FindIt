#include "NodeNotifyNewProductResponse.hpp"

namespace FindIt
{

NodeNotifyNewProductResponse::NodeNotifyNewProductResponse(std::string product, bool result)
    : product(product), result(result)
{
    this->type = MessageType::NODE_NOTIFY_NEW_PRODUCT_RESPONSE;
    this->action = "NotifyNewProduct";
}

MessageType NodeNotifyNewProductResponse::GetType()
{
    return this->type;
}

std::string NodeNotifyNewProductResponse::GetAction()
{
    return this->action;
}

std::string NodeNotifyNewProductResponse::GetProduct()
{
    return this->product;
}

bool NodeNotifyNewProductResponse::GetResult()
{
    return this->result;
}

};
