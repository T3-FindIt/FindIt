#ifndef NODEEVENTPRODUCTFOUND_HPP
#define NODEEVENTPRODUCTFOUND_HPP

#include <IMessage.hpp>

namespace FindIt
{

class NodeEventProductFound : public IMessage
{
private:
    std::string product;
    bool result;
public:
    NodeEventProductFound(std::string product, bool result);
    ~NodeEventProductFound() = default;

    MessageType GetType() final;
    std::string GetAction();
    std::string GetProduct();
    bool GetResult();
};

}

#endif // NODEEVENTPRODUCTFOUND_HPP
