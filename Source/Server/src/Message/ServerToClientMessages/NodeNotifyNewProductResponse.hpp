#ifndef NODENOTIFYNEWPRODUCTRESPONSE_HPP
#define NODENOTIFYNEWPRODUCTRESPONSE_HPP

#include <IMessage.hpp>

#include <string>

namespace FindIt
{

class NodeNotifyNewProductResponse : public IMessage
{
private:
    std::string product;
    bool result;
public:
    NodeNotifyNewProductResponse(std::string product, bool result);
    ~NodeNotifyNewProductResponse() = default;

    MessageType GetType() final;
    std::string GetAction();
    std::string GetProduct();
    bool GetResult();
};

}

#endif // NODENOTIFYNEWPRODUCTRESPONSE_HPP
