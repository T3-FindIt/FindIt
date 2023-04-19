#ifndef NODERESPONDTOPRODUCTREQUEST_HPP
#define NODERESPONDTOPRODUCTREQUEST_HPP

#include "../IMessage.hpp"

namespace FindIt
{

class NodeRespondToProductRequest : public IMessage
{
private:
    std::string product;
    bool result;
public:
    NodeRespondToProductRequest(std::string product, bool result);
    ~NodeRespondToProductRequest() = default;

    MessageType GetType() final;
    std::string GetAction();
    std::string GetProduct();
    bool GetResult();
};

}

#endif // NODERESPONDTOPRODUCTREQUEST_HPP
