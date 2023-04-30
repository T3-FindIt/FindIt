#ifndef NODENOTIFYNEWPRODUCT_HPP
#define NODENOTIFYNEWPRODUCT_HPP

#include <IMessage.hpp>

namespace FindIt
{

class NodeNotifyNewProduct : public IMessage
{
private:
    std::string product;
public:
    NodeNotifyNewProduct(std::string product);
    ~NodeNotifyNewProduct() = default;

    MessageType GetType() final;
    std::string GetAction();
    std::string GetProduct();
};

}

#endif // NODENOTIFYNEWPRODUCT_HPP
