#include "JSONProtocolParser.hpp"

#include <nlohmann/json.hpp>
#include <HeartBeat.hpp>

using json = nlohmann::json;

namespace FindIt
{

IMessage* JSONProtocolParser::Parse(std::string data)
{
    json json_obj;
    try
    {
        json_obj = json::parse(data);
    }
    catch(const nlohmann::json::parse_error& e)
    {
        std::cerr << e.what() << '\n';
        return nullptr;
    }

    if (json_obj.find("action") == json_obj.end())
    {
        return nullptr;
    }

    if (!json_obj["action"].is_string())
    {
        return nullptr;
    }

    std::string action = json_obj["action"];

    if (action == "HeartBeat")
    {
        if (json_obj.find("Node") != json_obj.end()
            && json_obj.find("Places") != json_obj.end())
        {
            return new HeartBeatResponse(json_obj["Node"], json_obj["Places"]);
        }
        else
        {
            return new HeartBeat();
        }
    }
    return nullptr;
}

std::string JSONProtocolParser::Parse(IMessage &data)
{
    return std::string();
}

};
