#include "JSONProtocolParser.hpp"

#include <nlohmann/json.hpp>
using json = nlohmann::json;
#include "../../Message/ServerToClientMessages/HeartBeat.hpp"


namespace FindIt
{

    IMessage* JSONProtocolParser::Parse(std::string data)
    {
        json test = json::parse(data);
        IMessage* message = new HeartBeat();
        return message;
    }

    std::string JSONProtocolParser::Parse(IMessage &data)
    {
        return std::string();
    }

};