#include "JSONProtocolParser.hpp"

#include <nlohmann/json.hpp>
#include <iostream>

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

    if (json_obj.find("Action") == json_obj.end())
    {
        return nullptr;
    }

    if (!json_obj["Action"].is_string())
    {
        return nullptr;
    }

    if (std::string action = json_obj["Action"]; action == "HeartBeat")
    {
        if (json_obj.find("Node") != json_obj.end()
            && json_obj.find("Places") != json_obj.end())
        {
            return new HeartBeatResponse(json_obj["Node"], json_obj["Places"]);
        }
        else if (json_obj.find("Node") == json_obj.end()
                && json_obj.find("Places") == json_obj.end())
        {
            return new HeartBeat();
        }
    }
    else if (action == "SignIn"
            && json_obj.find("Node") != json_obj.end()
            && json_obj.find("Places") != json_obj.end())
    {
        if (json_obj.find("Result") != json_obj.end())
        {
            return new NodeSignInResponse(json_obj["Node"], json_obj["Places"], json_obj["Result"]);
        }
        {
            return new NodeSignIn(json_obj["Node"], json_obj["Places"]);
        }
    }
    else if (action == "NotifyNewProduct"
            && json_obj.find("Product") != json_obj.end())
    {
        if (json_obj.find("Result") != json_obj.end())
        {
            return new NodeNotifyNewProductResponse(json_obj["Product"], json_obj["Result"]);
        }
        else
        {
            return new NodeNotifyNewProduct(json_obj["Product"]);
        }
    }
    else if (action == "RequestProduct"
            && json_obj.find("Product") != json_obj.end()
            && json_obj.find("_Activate") != json_obj.end())
    {
        return new ServerRequestProduct(json_obj["Product"], json_obj["_Activate"]);
    }
    else if (action == "ResponseProduct"
            && json_obj.find("Product") != json_obj.end()
            && json_obj.find("Result") != json_obj.end())
    {
        return new NodeRespondToProductRequest(json_obj["Product"], json_obj["Result"]);
    }
    else if (action == "ProductFound"
            && json_obj.find("Product") != json_obj.end()
            && json_obj.find("Result") != json_obj.end())
    {
        return new NodeEventProductFound(json_obj["Product"], json_obj["Result"]);
    }

    return nullptr;
}

std::string JSONProtocolParser::Parse(IMessage &data)
{
    return std::string();
}

};
