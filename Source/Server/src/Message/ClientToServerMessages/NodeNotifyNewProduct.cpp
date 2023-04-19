#include "NodeNotifyNewProduct.hpp"

namespace FindIt
{

NodeNotifyNewProduct::NodeNotifyNewProduct(std::string product)
    : product(product)
{
    this->action = "NotifyNewProduct";
    this->type = MessageType::NODE_NOTIFY_NEW_PRODUCT;
}

MessageType NodeNotifyNewProduct::GetType()
{
    return this->type;
}

std::string NodeNotifyNewProduct::GetAction()
{
    return this->action;
}

std::string NodeNotifyNewProduct::GetProduct()
{
    return this->product;
}

};
