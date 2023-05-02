#ifndef JSONPROTOCOLPARSER_HPP
#define JSONPROTOCOLPARSER_HPP

#include "IProtocolParser.hpp"

namespace FindIt
{

class JSONProtocolParser : public IProtocolParser
{
public:
    JSONProtocolParser() = default;
    ~JSONProtocolParser() = default;

    IMessage* Parse(std::string data) final;
    std::string Parse(IMessage& data) final;
};

};

#endif // JSONPROTOCOLPARSER_HPP