#include "JSONProtocolParser.hpp"

#include <nlohmann/json.hpp>
#include <iostream>

namespace FindIt
{

IMessage* JSONProtocolParser::Parse(std::string data)
{
    nlohmann::json json_obj;
    try
    {
        json_obj = nlohmann::json::parse(data);
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
            // TODO: Use smart pointers
            return new HeartBeatResponse(json_obj["Node"], json_obj["Places"]);
        }
        else if (json_obj.find("Node") == json_obj.end()
                && json_obj.find("Places") == json_obj.end())
        {
            // TODO: Use smart pointers
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
            && json_obj.find("Activate") != json_obj.end())
    {
        return new ServerRequestProduct(json_obj["Product"], json_obj["Activate"]);
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
    nlohmann::json json_obj;

    switch (data.GetType())
    {
    case MessageType::HEARTBEAT:
    {
        HeartBeat &heartbeat = dynamic_cast<HeartBeat&>(data);
        json_obj["Action"] = heartbeat.GetAction();
        break;
    }
    case MessageType::HEARTBEAT_RESPONSE:
    {
        HeartBeatResponse &heartbeat_response = dynamic_cast<HeartBeatResponse&>(data);
        json_obj["Action"] = heartbeat_response.GetAction();
        json_obj["Node"] = heartbeat_response.GetNode();
        json_obj["Places"] = heartbeat_response.GetPlaces();
        break;
    }
    case MessageType::NODE_SIGN_IN:
    {
        NodeSignIn &node_sign_in = dynamic_cast<NodeSignIn&>(data);
        json_obj["Action"] = node_sign_in.GetAction();
        json_obj["Node"] = node_sign_in.GetNode();
        json_obj["Places"] = node_sign_in.GetPlaces();
        break;
    }
    case MessageType::NODE_SIGN_IN_RESPONSE:
    {
        NodeSignInResponse &node_sign_in_response = dynamic_cast<NodeSignInResponse&>(data);
        json_obj["Action"] = node_sign_in_response.GetAction();
        json_obj["Node"] = node_sign_in_response.GetNode();
        json_obj["Places"] = node_sign_in_response.GetPlaces();
        json_obj["Result"] = node_sign_in_response.GetResult();
        break;
    }
    case MessageType::NODE_NOTIFY_NEW_PRODUCT:
    {
        NodeNotifyNewProduct &node_notify_new_product = dynamic_cast<NodeNotifyNewProduct&>(data);
        json_obj["Action"] = node_notify_new_product.GetAction();
        json_obj["Product"] = node_notify_new_product.GetProduct();
        break;
    }
    case MessageType::NODE_NOTIFY_NEW_PRODUCT_RESPONSE:
    {
        NodeNotifyNewProductResponse &node_notify_new_product_response = dynamic_cast<NodeNotifyNewProductResponse&>(data);
        json_obj["Action"] = node_notify_new_product_response.GetAction();
        json_obj["Product"] = node_notify_new_product_response.GetProduct();
        json_obj["Result"] = node_notify_new_product_response.GetResult();
        break;
    }
    case MessageType::SERVER_REQUEST_PRODUCT:
    {
        ServerRequestProduct &server_request_product = dynamic_cast<ServerRequestProduct&>(data);
        json_obj["Action"] = server_request_product.GetAction();
        json_obj["Product"] = server_request_product.GetProduct();
        json_obj["Activate"] = server_request_product.GetActivate();
        break;
    }
    case MessageType::NODE_RESPOND_TO_PRODUCT_REQUEST:
    {
        NodeRespondToProductRequest &node_respond_to_product_request = dynamic_cast<NodeRespondToProductRequest&>(data);
        json_obj["Action"] = node_respond_to_product_request.GetAction();
        json_obj["Product"] = node_respond_to_product_request.GetProduct();
        json_obj["Result"] = node_respond_to_product_request.GetResult();
        break;
    }
    case MessageType::NODE_EVENT_PRODUCT_FOUND:
    {
        NodeEventProductFound &node_event_product_found = dynamic_cast<NodeEventProductFound&>(data);
        json_obj["Action"] = node_event_product_found.GetAction();
        json_obj["Product"] = node_event_product_found.GetProduct();
        json_obj["Result"] = node_event_product_found.GetResult();
        break;
    }
    default:
    {
        return std::string("");
        break;
    }
    }

    return json_obj.dump();
}

};
