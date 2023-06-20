#ifndef JSONPROTOCOLPARSER_HPP
#define JSONPROTOCOLPARSER_HPP

#include "IProtocolParser.hpp"

#include <IMessage.hpp>

#include <HeartbeatResponse.hpp>
#include <NodeEventProductFound.hpp>
#include <NodeNotifyNewProduct.hpp>
#include <NodeRespondToProductRequest.hpp>
#include <NodeSignIn.hpp>

#include <HeartBeat.hpp>
#include <NodeNotifyNewProductResponse.hpp>
#include <NodeSignInResponse.hpp>
#include <ServerRequestProduct.hpp>

namespace FindIt
{

class JSONProtocolParser : public IProtocolParser
{
public:
    JSONProtocolParser() = default;
    ~JSONProtocolParser() = default;

    std::shared_ptr<IMessage> Parse(const std::string& data) final;
    std::string Parse(IMessage& data) final;
};

};

#endif // JSONPROTOCOLPARSER_HPP
