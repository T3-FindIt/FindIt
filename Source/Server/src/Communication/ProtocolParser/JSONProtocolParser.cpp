#include "JSONProtocolParser.hpp"

#include <nlohmann/json.hpp>
#include <HeartBeat.hpp>

using json = nlohmann::json;

namespace FindIt
{

IMessage* JSONProtocolParser::Parse(std::string data)
{
    // json test = json::parse(data);
    // IMessage* message = new HeartBeat();
    // return message;
    return nullptr;
}

std::string JSONProtocolParser::Parse(IMessage &data)
{
    return std::string();
}

};
