#ifndef SERVERREQUESTPRODUCT_HPP
#define SERVERREQUESTPRODUCT_HPP

#include <IMessage.hpp>

namespace FindIt
{

class ServerRequestProduct : public IMessage
{
private:
    std::string product;
    bool activate;
public:
    ServerRequestProduct(std::string product, bool activate);
    ~ServerRequestProduct() = default;

    MessageType GetType() final;
    std::string GetAction();
    std::string GetProduct();
    bool GetActivate();
};

}

#endif // SERVERREQUESTPRODUCT_HPP
