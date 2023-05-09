#include "ServerRequestProduct.hpp"

namespace FindIt
{

FindIt::ServerRequestProduct::ServerRequestProduct(std::string product, bool activate)
    : product(product), activate(activate)
{
    this->action = "RequestProduct";
    this->type = MessageType::SERVER_REQUEST_PRODUCT;
}

MessageType ServerRequestProduct::GetType()
{
    return this->type;
}

std::string ServerRequestProduct::GetAction()
{
    return this->action;
}

std::string ServerRequestProduct::GetProduct()
{
    return this->product;
}

bool ServerRequestProduct::GetActivate()
{
    return this->activate;
}
};
