#ifndef IPROTOCOLPARSER_HPP
#define IPROTOCOLPARSER_HPP

#include <IMessage.hpp>

#include <string>
#include <memory>

namespace FindIt
{

class IProtocolParser
{
public:
    virtual ~IProtocolParser() = default;
    virtual std::shared_ptr<IMessage> Parse(const std::string& data) = 0;
    virtual std::string Parse(IMessage& data) = 0;
};

};

#endif // IPROTOCOLPARSER_HPP
