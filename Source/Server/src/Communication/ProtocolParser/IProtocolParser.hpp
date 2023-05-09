#ifndef IPROTOCOLPARSER_HPP
#define IPROTOCOLPARSER_HPP

#include <IMessage.hpp>

#include <string>

namespace FindIt
{

class IProtocolParser
{
public:
    virtual IMessage* Parse(std::string data) = 0;
    virtual std::string Parse(IMessage& data) = 0;
};

};

#endif // IPROTOCOLPARSER_HPP
