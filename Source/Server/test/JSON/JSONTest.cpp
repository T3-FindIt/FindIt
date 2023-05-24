#include <gtest/gtest.h>
#include <JSONProtocolParser.hpp>

namespace
{

class JSONTests : public ::testing::Test
{
protected:
    FindIt::JSONProtocolParser parser;
};

//========================================================//
//                                                        //
//                     JSON -> Object                     //
//                                                        //
//========================================================//

TEST_F(JSONTests, EmptyStringParse)
{
    std::string data = "";
    FindIt::IMessage* message = parser.Parse(data);
    ASSERT_EQ(message, nullptr);
}

TEST_F(JSONTests, EmptyJSONParse)
{
    std::string data = "{}";
    FindIt::IMessage* message = parser.Parse(data);
    ASSERT_EQ(message, nullptr);
}

TEST_F(JSONTests, ParseHeartBeatToObject)
{
    std::string data = "{\"Action\":\"HeartBeat\"}";
    FindIt::IMessage* message = parser.Parse(data);
    ASSERT_NE(message, nullptr);
    ASSERT_EQ(message->GetType(), FindIt::MessageType::HEARTBEAT);
    FindIt::HeartBeat* heartbeat = dynamic_cast<FindIt::HeartBeat*>(message);
    ASSERT_EQ(heartbeat->GetAction(), "HeartBeat");
    delete message;
}

TEST_F(JSONTests, ParseHeartBeatToObjectLowercaseFail)
{
    std::string data = "{\"Action\":\"heartbeat\"}";
    FindIt::IMessage* message = parser.Parse(data);
    ASSERT_EQ(message, nullptr);
}

TEST_F(JSONTests, ParseHeartBeatResponseToObject)
{
    std::string data = "{\"Action\":\"HeartBeat\",\"Node\":\"NodeName\",\"Places\":3}";
    FindIt::IMessage* message = parser.Parse(data);
    ASSERT_NE(message, nullptr);
    ASSERT_EQ(message->GetType(), FindIt::MessageType::HEARTBEAT_RESPONSE);
    FindIt::HeartBeatResponse* heartbeatResponse = dynamic_cast<FindIt::HeartBeatResponse*>(message);
    ASSERT_EQ(heartbeatResponse->GetAction(), "HeartBeat");
    ASSERT_EQ(heartbeatResponse->GetNode(), "NodeName");
    ASSERT_EQ(heartbeatResponse->GetPlaces(), 3);
    delete message;
}

TEST_F(JSONTests, ParseHeartBeatResponseToObjectLowerCaseActionFail)
{
    std::string data = "{\"Action\":\"heartbeat\",\"Node\":\"NodeName\",\"Places\":3}";
    FindIt::IMessage* message = parser.Parse(data);
    ASSERT_EQ(message, nullptr);
}

TEST_F(JSONTests, ParseHeartBeatResponseToObjectMissingNodeFail)
{
    std::string data = "{\"Action\":\"HeartBeat\",\"Places\":3}";
    FindIt::IMessage* message = parser.Parse(data);
    ASSERT_EQ(message, nullptr);
}

TEST_F(JSONTests, ParseHeartBeatResponseToObjectMissingPlacesFail)
{
    std::string data = "{\"Action\":\"HeartBeat\",\"Node\":\"NodeName\"}";
    FindIt::IMessage* message = parser.Parse(data);
    ASSERT_EQ(message, nullptr);
}

TEST_F(JSONTests, ParseSignInToObject)
{
    std::string data = "{\"Action\":\"SignIn\",\"Node\":\"NodeName\",\"Places\":3}";
    FindIt::IMessage* message = parser.Parse(data);
    ASSERT_NE(message, nullptr);
    ASSERT_EQ(message->GetType(), FindIt::MessageType::NODE_SIGN_IN);
    FindIt::NodeSignIn* signIn = dynamic_cast<FindIt::NodeSignIn*>(message);
    ASSERT_EQ(signIn->GetAction(), "SignIn");
    ASSERT_EQ(signIn->GetNode(), "NodeName");
    ASSERT_EQ(signIn->GetPlaces(), 3);
    delete message;
}

TEST_F(JSONTests, ParseSignInToObjectLowerCaseActionFail)
{
    std::string data = "{\"Action\":\"signin\",\"Node\":\"NodeName\",\"Places\":3}";
    FindIt::IMessage* message = parser.Parse(data);
    ASSERT_EQ(message, nullptr);
}

TEST_F(JSONTests, ParseSignInToObjectMissingNodeFail)
{
    std::string data = "{\"Action\":\"SignIn\",\"Places\":3}";
    FindIt::IMessage* message = parser.Parse(data);
    ASSERT_EQ(message, nullptr);
}

TEST_F(JSONTests, ParseSignInToObjectMissingPlacesFail)
{
    std::string data = "{\"Action\":\"SignIn\",\"Node\":\"NodeName\"}";
    FindIt::IMessage* message = parser.Parse(data);
    ASSERT_EQ(message, nullptr);
}

TEST_F(JSONTests, ParseSignInResponseToObject)
{
    std::string data = "{\"Action\":\"SignIn\",\"Node\":\"NodeName\",\"Places\":3,\"Result\":true}";
    FindIt::IMessage* message = parser.Parse(data);
    ASSERT_NE(message, nullptr);
    ASSERT_EQ(message->GetType(), FindIt::MessageType::NODE_SIGN_IN_RESPONSE);
    FindIt::NodeSignInResponse* signInResponse = dynamic_cast<FindIt::NodeSignInResponse*>(message);
    ASSERT_EQ(signInResponse->GetAction(), "SignIn");
    ASSERT_EQ(signInResponse->GetNode(), "NodeName");
    ASSERT_EQ(signInResponse->GetPlaces(), 3);
    ASSERT_EQ(signInResponse->GetResult(), true);
    delete message;
}

TEST_F(JSONTests, ParseNodeNotifyNewProductResponseToObject)
{
    std::string data = "{\"Action\":\"NotifyNewProduct\",\"Product\":\"Product\",\"Result\":true}";
    FindIt::IMessage* message = parser.Parse(data);
    ASSERT_NE(message, nullptr);
    ASSERT_EQ(message->GetType(), FindIt::MessageType::NODE_NOTIFY_NEW_PRODUCT_RESPONSE);
    FindIt::NodeNotifyNewProductResponse* nodeNotifyNewProductResponse = dynamic_cast<FindIt::NodeNotifyNewProductResponse*>(message);
    ASSERT_EQ(nodeNotifyNewProductResponse->GetAction(), "NotifyNewProduct");
    ASSERT_EQ(nodeNotifyNewProductResponse->GetProduct(), "Product");
    ASSERT_EQ(nodeNotifyNewProductResponse->GetResult(), true);
    delete message;
}

TEST_F(JSONTests, ParseNodeNotifyNewProductResponseToObjectLowerCaseActionFail)
{
    std::string data = "{\"Action\":\"notifynewproduct\",\"Product\":\"Product\",\"Result\":true}";
    FindIt::IMessage* message = parser.Parse(data);
    ASSERT_EQ(message, nullptr);
}

TEST_F(JSONTests, ParseNodeNotifyNewProductResponseToObjectMissingProductFail)
{
    std::string data = "{\"Action\":\"NotifyNewProduct\",\"Result\":true}";
    FindIt::IMessage* message = parser.Parse(data);
    ASSERT_EQ(message, nullptr);
}

TEST_F(JSONTests, ParseNodeNotifyNewProductToObject)
{
    std::string data = "{\"Action\":\"NotifyNewProduct\",\"Product\":\"Product\"}";
    FindIt::IMessage* message = parser.Parse(data);
    ASSERT_NE(message, nullptr);
    ASSERT_EQ(message->GetType(), FindIt::MessageType::NODE_NOTIFY_NEW_PRODUCT);
    FindIt::NodeNotifyNewProduct* nodeNotifyNewProduct = dynamic_cast<FindIt::NodeNotifyNewProduct*>(message);
    ASSERT_EQ(nodeNotifyNewProduct->GetAction(), "NotifyNewProduct");
    ASSERT_EQ(nodeNotifyNewProduct->GetProduct(), "Product");
    delete message;
}

TEST_F(JSONTests, ParseServerRequestProductToObject)
{
    std::string data = "{\"Action\":\"RequestProduct\",\"Product\":\"Product\",\"_Activate\":true}";
    FindIt::IMessage* message = parser.Parse(data);
    ASSERT_NE(message, nullptr);
    ASSERT_EQ(message->GetType(), FindIt::MessageType::SERVER_REQUEST_PRODUCT);
    FindIt::ServerRequestProduct* serverRequestProduct = dynamic_cast<FindIt::ServerRequestProduct*>(message);
    ASSERT_EQ(serverRequestProduct->GetAction(), "RequestProduct");
    ASSERT_EQ(serverRequestProduct->GetProduct(), "Product");
    ASSERT_EQ(serverRequestProduct->GetActivate(), true);
    delete message;
}

TEST_F(JSONTests, ParseServerRequestProductToObjectLowerCaseActionFail)
{
    std::string data = "{\"Action\":\"requestproduct\",\"Product\":\"Product\",\"_Activate\":true}";
    FindIt::IMessage* message = parser.Parse(data);
    ASSERT_EQ(message, nullptr);
}

TEST_F(JSONTests, ParseServerRequestProductToObjectMissingProductFail)
{
    std::string data = "{\"Action\":\"RequestProduct\",\"_Activate\":true}";
    FindIt::IMessage* message = parser.Parse(data);
    ASSERT_EQ(message, nullptr);
}

TEST_F(JSONTests, ParseServerRequestProductToObjectMissingActivateFail)
{
    std::string data = "{\"Action\":\"RequestProduct\",\"Product\":\"Product\"}";
    FindIt::IMessage* message = parser.Parse(data);
    ASSERT_EQ(message, nullptr);
}

TEST_F(JSONTests, ParseNodeRespondToProductRequestToObject)
{
    std::string data = "{\"Action\":\"ResponseProduct\",\"Product\":\"Product\",\"Result\":true}";
    FindIt::IMessage* message = parser.Parse(data);
    ASSERT_NE(message, nullptr);
    ASSERT_EQ(message->GetType(), FindIt::MessageType::NODE_RESPOND_TO_PRODUCT_REQUEST);
    FindIt::NodeRespondToProductRequest* nodeRespondToProductRequest = dynamic_cast<FindIt::NodeRespondToProductRequest*>(message);
    ASSERT_EQ(nodeRespondToProductRequest->GetAction(), "ResponseProduct");
    ASSERT_EQ(nodeRespondToProductRequest->GetProduct(), "Product");
    ASSERT_EQ(nodeRespondToProductRequest->GetResult(), true);
    delete message;
}

TEST_F(JSONTests, ParseNodeRespondToProductRequestToObjectLowerCaseActionFail)
{
    std::string data = "{\"Action\":\"responseproduct\",\"Product\":\"Product\",\"Result\":true}";
    FindIt::IMessage* message = parser.Parse(data);
    ASSERT_EQ(message, nullptr);
}

TEST_F(JSONTests, ParseNodeRespondToProductRequestToObjectMissingProductFail)
{
    std::string data = "{\"Action\":\"ResponseProduct\",\"Result\":true}";
    FindIt::IMessage* message = parser.Parse(data);
    ASSERT_EQ(message, nullptr);
}

TEST_F(JSONTests, ParseNodeRespondToProductRequestToObjectMissingResultFail)
{
    std::string data = "{\"Action\":\"ResponseProduct\",\"Product\":\"Product\"}";
    FindIt::IMessage* message = parser.Parse(data);
    ASSERT_EQ(message, nullptr);
}

TEST_F(JSONTests, ParseNodeEventProductFoundToObject)
{
    std::string data = "{\"Action\":\"ProductFound\",\"Product\":\"Product\",\"Result\":true}";
    FindIt::IMessage* message = parser.Parse(data);
    ASSERT_NE(message, nullptr);
    ASSERT_EQ(message->GetType(), FindIt::MessageType::NODE_EVENT_PRODUCT_FOUND);
    FindIt::NodeEventProductFound* nodeEventProductFound = dynamic_cast<FindIt::NodeEventProductFound*>(message);
    ASSERT_EQ(nodeEventProductFound->GetAction(), "ProductFound");
    ASSERT_EQ(nodeEventProductFound->GetProduct(), "Product");
    ASSERT_EQ(nodeEventProductFound->GetResult(), true);
    delete message;
}

TEST_F(JSONTests, ParseNodeEventProductFoundToObjectLowerCaseActionFail)
{
    std::string data = "{\"Action\":\"productfound\",\"Product\":\"Product\",\"Result\":true}";
    FindIt::IMessage* message = parser.Parse(data);
    ASSERT_EQ(message, nullptr);
}

TEST_F(JSONTests, ParseNodeEventProductFoundToObjectMissingProductFail)
{
    std::string data = "{\"Action\":\"ProductFound\",\"Result\":true}";
    FindIt::IMessage* message = parser.Parse(data);
    ASSERT_EQ(message, nullptr);
}

TEST_F(JSONTests, ParseNodeEventProductFoundToObjectMissingResultFail)
{
    std::string data = "{\"Action\":\"ProductFound\",\"Product\":\"Product\"}";
    FindIt::IMessage* message = parser.Parse(data);
    ASSERT_EQ(message, nullptr);
}

//========================================================//
//                                                        //
//                     Object -> JSON                     //
//                                                        //
//========================================================//

TEST_F(JSONTests, ParseHeartBeatToJSON)
{
    FindIt::HeartBeat heartBeat;
    std::string data = parser.Parse(heartBeat);
    ASSERT_EQ(data, "{\"Action\":\"HeartBeat\"}");
}

TEST_F(JSONTests, ParseHeartBeatResponseToJSON)
{
    FindIt::HeartBeatResponse heartBeatResponse("NodeName", 3);
    std::string data = parser.Parse(heartBeatResponse);
    ASSERT_EQ(data, "{\"Action\":\"HeartBeat\",\"Node\":\"NodeName\",\"Places\":3}");
}

TEST_F(JSONTests, ParseSignInToJSON)
{
    FindIt::NodeSignIn signIn("NodeName", 3);
    std::string data = parser.Parse(signIn);
    ASSERT_EQ(data, "{\"Action\":\"SignIn\",\"Node\":\"NodeName\",\"Places\":3}");
}

TEST_F(JSONTests, ParseSignInResponseToJSON)
{
    FindIt::NodeSignInResponse signInResponse("NodeName", 3, true);
    std::string data = parser.Parse(signInResponse);
    ASSERT_EQ(data, "{\"Action\":\"SignIn\",\"Node\":\"NodeName\",\"Places\":3,\"Result\":true}");
}

TEST_F(JSONTests, ParseNodeNotifyNewProductResponseToJSON)
{
    FindIt::NodeNotifyNewProductResponse nodeNotifyNewProductResponse("Product", true);
    std::string data = parser.Parse(nodeNotifyNewProductResponse);
    ASSERT_EQ(data, "{\"Action\":\"NotifyNewProduct\",\"Product\":\"Product\",\"Result\":true}");
}

TEST_F(JSONTests, ParseNodeNotifyNewProductToJSON)
{
    FindIt::NodeNotifyNewProduct nodeNotifyNewProduct("Product");
    std::string data = parser.Parse(nodeNotifyNewProduct);
    ASSERT_EQ(data, "{\"Action\":\"NotifyNewProduct\",\"Product\":\"Product\"}");
}

TEST_F(JSONTests, ParseServerRequestProductToJSON)
{
    FindIt::ServerRequestProduct serverRequestProduct("Product", false);
    std::string data = parser.Parse(serverRequestProduct);
    ASSERT_EQ(data, "{\"Action\":\"RequestProduct\",\"Product\":\"Product\",\"_Activate\":false}");
}

TEST_F(JSONTests, ParseNodeRespondToProductRequestToJSON)
{
    FindIt::NodeRespondToProductRequest nodeRespondToProductRequest("Product", true);
    std::string data = parser.Parse(nodeRespondToProductRequest);
    ASSERT_EQ(data, "{\"Action\":\"ResponseProduct\",\"Product\":\"Product\",\"Result\":true}");
}

TEST_F(JSONTests, ParseNodeEventProductFoundToJSON)
{
    FindIt::NodeEventProductFound nodeEventProductFound("Product", true);
    std::string data = parser.Parse(nodeEventProductFound);
    ASSERT_EQ(data, "{\"Action\":\"ProductFound\",\"Product\":\"Product\",\"Result\":true}");
}

//========================================================//
//                                                        //
//                Object -> JSON -> Object                //
//                                                        //
//========================================================//

TEST_F(JSONTests, ParseHeartBeatToJSONToObject)
{
    FindIt::HeartBeat heartBeat;
    std::string data = parser.Parse(heartBeat);
    FindIt::IMessage* message = parser.Parse(data);
    ASSERT_NE(message, nullptr);
    ASSERT_EQ(message->GetType(), FindIt::MessageType::HEARTBEAT);
    delete message;
}

TEST_F(JSONTests, ParseHeartBeatResponseToJSONToObject)
{
    FindIt::HeartBeatResponse heartBeatResponse("NodeName", 3);
    std::string data = parser.Parse(heartBeatResponse);
    FindIt::IMessage* message = parser.Parse(data);
    ASSERT_NE(message, nullptr);
    ASSERT_EQ(message->GetType(), FindIt::MessageType::HEARTBEAT_RESPONSE);
    FindIt::HeartBeatResponse* heartBeatResponse2 = dynamic_cast<FindIt::HeartBeatResponse*>(message);
    ASSERT_EQ(heartBeatResponse2->GetAction(), "HeartBeat");
    ASSERT_EQ(heartBeatResponse2->GetNode(), "NodeName");
    ASSERT_EQ(heartBeatResponse2->GetPlaces(), 3);
    delete message;
}

TEST_F(JSONTests, ParseSignInToJSONToObject)
{
    FindIt::NodeSignIn signIn("NodeName", 3);
    std::string data = parser.Parse(signIn);
    FindIt::IMessage* message = parser.Parse(data);
    ASSERT_NE(message, nullptr);
    ASSERT_EQ(message->GetType(), FindIt::MessageType::NODE_SIGN_IN);
    FindIt::NodeSignIn* signIn2 = dynamic_cast<FindIt::NodeSignIn*>(message);
    ASSERT_EQ(signIn2->GetAction(), "SignIn");
    ASSERT_EQ(signIn2->GetNode(), "NodeName");
    ASSERT_EQ(signIn2->GetPlaces(), 3);
    delete message;
}

TEST_F(JSONTests, ParseSignInResponseToJSONToObject)
{
    FindIt::NodeSignInResponse signInResponse("NodeName", 3, true);
    std::string data = parser.Parse(signInResponse);
    FindIt::IMessage* message = parser.Parse(data);
    ASSERT_NE(message, nullptr);
    ASSERT_EQ(message->GetType(), FindIt::MessageType::NODE_SIGN_IN_RESPONSE);
    FindIt::NodeSignInResponse* signInResponse2 = dynamic_cast<FindIt::NodeSignInResponse*>(message);
    ASSERT_EQ(signInResponse2->GetAction(), "SignIn");
    ASSERT_EQ(signInResponse2->GetNode(), "NodeName");
    ASSERT_EQ(signInResponse2->GetPlaces(), 3);
    ASSERT_EQ(signInResponse2->GetResult(), true);
    delete message;
}

TEST_F(JSONTests, ParseNodeNotifyNewProductResponseToJSONToObject)
{
    FindIt::NodeNotifyNewProductResponse nodeNotifyNewProductResponse("Product", true);
    std::string data = parser.Parse(nodeNotifyNewProductResponse);
    FindIt::IMessage* message = parser.Parse(data);
    ASSERT_NE(message, nullptr);
    ASSERT_EQ(message->GetType(), FindIt::MessageType::NODE_NOTIFY_NEW_PRODUCT_RESPONSE);
    FindIt::NodeNotifyNewProductResponse* nodeNotifyNewProductResponse2 = dynamic_cast<FindIt::NodeNotifyNewProductResponse*>(message);
    ASSERT_EQ(nodeNotifyNewProductResponse2->GetAction(), "NotifyNewProduct");
    ASSERT_EQ(nodeNotifyNewProductResponse2->GetProduct(), "Product");
    ASSERT_EQ(nodeNotifyNewProductResponse2->GetResult(), true);
    delete message;
}

TEST_F(JSONTests, ParseNodeNotifyNewProductToJSONToObject)
{
    FindIt::NodeNotifyNewProduct nodeNotifyNewProduct("Product");
    std::string data = parser.Parse(nodeNotifyNewProduct);
    FindIt::IMessage* message = parser.Parse(data);
    ASSERT_NE(message, nullptr);
    ASSERT_EQ(message->GetType(), FindIt::MessageType::NODE_NOTIFY_NEW_PRODUCT);
    FindIt::NodeNotifyNewProduct* nodeNotifyNewProduct2 = dynamic_cast<FindIt::NodeNotifyNewProduct*>(message);
    ASSERT_EQ(nodeNotifyNewProduct2->GetAction(), "NotifyNewProduct");
    ASSERT_EQ(nodeNotifyNewProduct2->GetProduct(), "Product");
    delete message;
}

TEST_F(JSONTests, ParseServerRequestProductToJSONToObject)
{
    FindIt::ServerRequestProduct serverRequestProduct("Product", false);
    std::string data = parser.Parse(serverRequestProduct);
    FindIt::IMessage* message = parser.Parse(data);
    ASSERT_NE(message, nullptr);
    ASSERT_EQ(message->GetType(), FindIt::MessageType::SERVER_REQUEST_PRODUCT);
    FindIt::ServerRequestProduct* serverRequestProduct2 = dynamic_cast<FindIt::ServerRequestProduct*>(message);
    ASSERT_EQ(serverRequestProduct2->GetAction(), "RequestProduct");
    ASSERT_EQ(serverRequestProduct2->GetProduct(), "Product");
    ASSERT_EQ(serverRequestProduct2->GetActivate(), false);
    delete message;
}

TEST_F(JSONTests, ParseNodeRespondToProductRequestToJSONToObject)
{
    FindIt::NodeRespondToProductRequest nodeRespondToProductRequest("Product", true);
    std::string data = parser.Parse(nodeRespondToProductRequest);
    FindIt::IMessage* message = parser.Parse(data);
    ASSERT_NE(message, nullptr);
    ASSERT_EQ(message->GetType(), FindIt::MessageType::NODE_RESPOND_TO_PRODUCT_REQUEST);
    FindIt::NodeRespondToProductRequest* nodeRespondToProductRequest2 = dynamic_cast<FindIt::NodeRespondToProductRequest*>(message);
    ASSERT_EQ(nodeRespondToProductRequest2->GetAction(), "ResponseProduct");
    ASSERT_EQ(nodeRespondToProductRequest2->GetProduct(), "Product");
    ASSERT_EQ(nodeRespondToProductRequest2->GetResult(), true);
    delete message;
}

TEST_F(JSONTests, ParseNodeEventProductFoundToJSONToObject)
{
    FindIt::NodeEventProductFound nodeEventProductFound("Product", true);
    std::string data = parser.Parse(nodeEventProductFound);
    FindIt::IMessage* message = parser.Parse(data);
    ASSERT_NE(message, nullptr);
    ASSERT_EQ(message->GetType(), FindIt::MessageType::NODE_EVENT_PRODUCT_FOUND);
    FindIt::NodeEventProductFound* nodeEventProductFound2 = dynamic_cast<FindIt::NodeEventProductFound*>(message);
    ASSERT_EQ(nodeEventProductFound2->GetAction(), "ProductFound");
    ASSERT_EQ(nodeEventProductFound2->GetProduct(), "Product");
    ASSERT_EQ(nodeEventProductFound2->GetResult(), true);
    delete message;
}

} // namespace
