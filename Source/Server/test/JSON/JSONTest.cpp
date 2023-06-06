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
    std::shared_ptr<FindIt::IMessage> message = parser.Parse(data);
    ASSERT_EQ(message, nullptr);
}

TEST_F(JSONTests, EmptyJSONParse)
{
    std::string data = "{}";
    std::shared_ptr<FindIt::IMessage> message = parser.Parse(data);
    ASSERT_EQ(message, nullptr);
}

TEST_F(JSONTests, ParseHeartBeatToObject)
{
    std::string data = "{\"Action\":\"HeartBeat\"}";
    std::shared_ptr<FindIt::IMessage> message = parser.Parse(data);
    ASSERT_NE(message, nullptr);
    ASSERT_EQ(message->GetType(), FindIt::MessageType::HEARTBEAT);
    std::shared_ptr<FindIt::HeartBeat> heartbeat = std::dynamic_pointer_cast<FindIt::HeartBeat>(message);
    ASSERT_EQ(heartbeat->GetAction(), "HeartBeat");
}

TEST_F(JSONTests, ParseHeartBeatToObjectLowercaseFail)
{
    std::string data = "{\"Action\":\"heartbeat\"}";
    std::shared_ptr<FindIt::IMessage> message = parser.Parse(data);
    ASSERT_EQ(message, nullptr);
}

TEST_F(JSONTests, ParseHeartBeatResponseToObject)
{
    std::string data = "{\"Action\":\"HeartBeat\",\"Node\":\"NodeName\",\"Places\":3}";
    std::shared_ptr<FindIt::IMessage> message = parser.Parse(data);
    ASSERT_NE(message, nullptr);
    ASSERT_EQ(message->GetType(), FindIt::MessageType::HEARTBEAT_RESPONSE);
    std::shared_ptr<FindIt::HeartBeatResponse> heartbeatResponse = std::dynamic_pointer_cast<FindIt::HeartBeatResponse>(message);
    ASSERT_EQ(heartbeatResponse->GetAction(), "HeartBeat");
    ASSERT_EQ(heartbeatResponse->GetNode(), "NodeName");
    ASSERT_EQ(heartbeatResponse->GetPlaces(), 3);
}

TEST_F(JSONTests, ParseHeartBeatResponseToObjectLowerCaseActionFail)
{
    std::string data = "{\"Action\":\"heartbeat\",\"Node\":\"NodeName\",\"Places\":3}";
    std::shared_ptr<FindIt::IMessage> message = parser.Parse(data);
    ASSERT_EQ(message, nullptr);
}

TEST_F(JSONTests, ParseHeartBeatResponseToObjectMissingNodeFail)
{
    std::string data = "{\"Action\":\"HeartBeat\",\"Places\":3}";
    std::shared_ptr<FindIt::IMessage> message = parser.Parse(data);
    ASSERT_EQ(message, nullptr);
}

TEST_F(JSONTests, ParseHeartBeatResponseToObjectMissingPlacesFail)
{
    std::string data = "{\"Action\":\"HeartBeat\",\"Node\":\"NodeName\"}";
    std::shared_ptr<FindIt::IMessage> message = parser.Parse(data);
    ASSERT_EQ(message, nullptr);
}

TEST_F(JSONTests, ParseSignInToObject)
{
    std::string data = "{\"Action\":\"SignIn\",\"Node\":\"NodeName\",\"Places\":3}";
    std::shared_ptr<FindIt::IMessage> message = parser.Parse(data);
    ASSERT_NE(message, nullptr);
    ASSERT_EQ(message->GetType(), FindIt::MessageType::NODE_SIGN_IN);
    std::shared_ptr<FindIt::NodeSignIn> signIn = std::dynamic_pointer_cast<FindIt::NodeSignIn>(message);
    ASSERT_EQ(signIn->GetAction(), "SignIn");
    ASSERT_EQ(signIn->GetNode(), "NodeName");
    ASSERT_EQ(signIn->GetPlaces(), 3);
}

TEST_F(JSONTests, ParseSignInToObjectLowerCaseActionFail)
{
    std::string data = "{\"Action\":\"signin\",\"Node\":\"NodeName\",\"Places\":3}";
    std::shared_ptr<FindIt::IMessage> message = parser.Parse(data);
    ASSERT_EQ(message, nullptr);
}

TEST_F(JSONTests, ParseSignInToObjectMissingNodeFail)
{
    std::string data = "{\"Action\":\"SignIn\",\"Places\":3}";
    std::shared_ptr<FindIt::IMessage> message = parser.Parse(data);
    ASSERT_EQ(message, nullptr);
}

TEST_F(JSONTests, ParseSignInToObjectMissingPlacesFail)
{
    std::string data = "{\"Action\":\"SignIn\",\"Node\":\"NodeName\"}";
    std::shared_ptr<FindIt::IMessage> message = parser.Parse(data);
    ASSERT_EQ(message, nullptr);
}

TEST_F(JSONTests, ParseSignInResponseToObject)
{
    std::string data = "{\"Action\":\"SignIn\",\"Node\":\"NodeName\",\"Places\":3,\"Result\":true}";
    std::shared_ptr<FindIt::IMessage> message = parser.Parse(data);
    ASSERT_NE(message, nullptr);
    ASSERT_EQ(message->GetType(), FindIt::MessageType::NODE_SIGN_IN_RESPONSE);
    std::shared_ptr<FindIt::NodeSignInResponse> signInResponse = std::dynamic_pointer_cast<FindIt::NodeSignInResponse>(message);
    ASSERT_EQ(signInResponse->GetAction(), "SignIn");
    ASSERT_EQ(signInResponse->GetNode(), "NodeName");
    ASSERT_EQ(signInResponse->GetPlaces(), 3);
    ASSERT_EQ(signInResponse->GetResult(), true);
}

TEST_F(JSONTests, ParseNodeNotifyNewProductResponseToObject)
{
    std::string data = "{\"Action\":\"NotifyNewProduct\",\"Product\":\"Product\",\"Result\":true}";
    std::shared_ptr<FindIt::IMessage> message = parser.Parse(data);
    ASSERT_NE(message, nullptr);
    ASSERT_EQ(message->GetType(), FindIt::MessageType::NODE_NOTIFY_NEW_PRODUCT_RESPONSE);
    std::shared_ptr<FindIt::NodeNotifyNewProductResponse> nodeNotifyNewProductResponse = std::dynamic_pointer_cast<FindIt::NodeNotifyNewProductResponse>(message);
    ASSERT_EQ(nodeNotifyNewProductResponse->GetAction(), "NotifyNewProduct");
    ASSERT_EQ(nodeNotifyNewProductResponse->GetProduct(), "Product");
    ASSERT_EQ(nodeNotifyNewProductResponse->GetResult(), true);
}

TEST_F(JSONTests, ParseNodeNotifyNewProductResponseToObjectLowerCaseActionFail)
{
    std::string data = "{\"Action\":\"notifynewproduct\",\"Product\":\"Product\",\"Result\":true}";
    std::shared_ptr<FindIt::IMessage> message = parser.Parse(data);
    ASSERT_EQ(message, nullptr);
}

TEST_F(JSONTests, ParseNodeNotifyNewProductResponseToObjectMissingProductFail)
{
    std::string data = "{\"Action\":\"NotifyNewProduct\",\"Result\":true}";
    std::shared_ptr<FindIt::IMessage> message = parser.Parse(data);
    ASSERT_EQ(message, nullptr);
}

TEST_F(JSONTests, ParseNodeNotifyNewProductToObject)
{
    std::string data = "{\"Action\":\"NotifyNewProduct\",\"Product\":\"Product\"}";
    std::shared_ptr<FindIt::IMessage> message = parser.Parse(data);
    ASSERT_NE(message, nullptr);
    ASSERT_EQ(message->GetType(), FindIt::MessageType::NODE_NOTIFY_NEW_PRODUCT);
    std::shared_ptr<FindIt::NodeNotifyNewProduct> nodeNotifyNewProduct = std::dynamic_pointer_cast<FindIt::NodeNotifyNewProduct>(message);
    ASSERT_EQ(nodeNotifyNewProduct->GetAction(), "NotifyNewProduct");
    ASSERT_EQ(nodeNotifyNewProduct->GetProduct(), "Product");
}

TEST_F(JSONTests, ParseServerRequestProductToObject)
{
    std::string data = "{\"Action\":\"RequestProduct\",\"Product\":\"Product\",\"Activate\":true}";
    std::shared_ptr<FindIt::IMessage> message = parser.Parse(data);
    ASSERT_NE(message, nullptr);
    ASSERT_EQ(message->GetType(), FindIt::MessageType::SERVER_REQUEST_PRODUCT);
    std::shared_ptr<FindIt::ServerRequestProduct> serverRequestProduct = std::dynamic_pointer_cast<FindIt::ServerRequestProduct>(message);
    ASSERT_EQ(serverRequestProduct->GetAction(), "RequestProduct");
    ASSERT_EQ(serverRequestProduct->GetProduct(), "Product");
    ASSERT_EQ(serverRequestProduct->GetActivate(), true);
}

TEST_F(JSONTests, ParseServerRequestProductToObjectDifferentOrderString)
{
    std::string data = "{\"Product\":\"Product\",\"Action\":\"RequestProduct\",\"Activate\":true}";
    std::shared_ptr<FindIt::IMessage> message = parser.Parse(data);
    ASSERT_NE(message, nullptr);
    ASSERT_EQ(message->GetType(), FindIt::MessageType::SERVER_REQUEST_PRODUCT);
    std::shared_ptr<FindIt::ServerRequestProduct> serverRequestProduct = std::dynamic_pointer_cast<FindIt::ServerRequestProduct>(message);
    ASSERT_EQ(serverRequestProduct->GetAction(), "RequestProduct");
    ASSERT_EQ(serverRequestProduct->GetProduct(), "Product");
    ASSERT_EQ(serverRequestProduct->GetActivate(), true);
}

TEST_F(JSONTests, ParseServerRequestProductToObjectLowerCaseActionFail)
{
    std::string data = "{\"Action\":\"requestproduct\",\"Product\":\"Product\",\"Activate\":true}";
    std::shared_ptr<FindIt::IMessage> message = parser.Parse(data);
    ASSERT_EQ(message, nullptr);
}

TEST_F(JSONTests, ParseServerRequestProductToObjectMissingProductFail)
{
    std::string data = "{\"Action\":\"RequestProduct\",\"Activate\":true}";
    std::shared_ptr<FindIt::IMessage> message = parser.Parse(data);
    ASSERT_EQ(message, nullptr);
}

TEST_F(JSONTests, ParseServerRequestProductToObjectMissingActivateFail)
{
    std::string data = "{\"Action\":\"RequestProduct\",\"Product\":\"Product\"}";
    std::shared_ptr<FindIt::IMessage> message = parser.Parse(data);
    ASSERT_EQ(message, nullptr);
}

TEST_F(JSONTests, ParseNodeRespondToProductRequestToObject)
{
    std::string data = "{\"Action\":\"ResponseProduct\",\"Product\":\"Product\",\"Result\":true}";
    std::shared_ptr<FindIt::IMessage> message = parser.Parse(data);
    ASSERT_NE(message, nullptr);
    ASSERT_EQ(message->GetType(), FindIt::MessageType::NODE_RESPOND_TO_PRODUCT_REQUEST);
    std::shared_ptr<FindIt::NodeRespondToProductRequest> nodeRespondToProductRequest = std::dynamic_pointer_cast<FindIt::NodeRespondToProductRequest>(message);
    ASSERT_EQ(nodeRespondToProductRequest->GetAction(), "ResponseProduct");
    ASSERT_EQ(nodeRespondToProductRequest->GetProduct(), "Product");
    ASSERT_EQ(nodeRespondToProductRequest->GetResult(), true);
}

TEST_F(JSONTests, ParseNodeRespondToProductRequestToObjectLowerCaseActionFail)
{
    std::string data = "{\"Action\":\"responseproduct\",\"Product\":\"Product\",\"Result\":true}";
    std::shared_ptr<FindIt::IMessage> message = parser.Parse(data);
    ASSERT_EQ(message, nullptr);
}

TEST_F(JSONTests, ParseNodeRespondToProductRequestToObjectMissingProductFail)
{
    std::string data = "{\"Action\":\"ResponseProduct\",\"Result\":true}";
    std::shared_ptr<FindIt::IMessage> message = parser.Parse(data);
    ASSERT_EQ(message, nullptr);
}

TEST_F(JSONTests, ParseNodeRespondToProductRequestToObjectMissingResultFail)
{
    std::string data = "{\"Action\":\"ResponseProduct\",\"Product\":\"Product\"}";
    std::shared_ptr<FindIt::IMessage> message = parser.Parse(data);
    ASSERT_EQ(message, nullptr);
}

TEST_F(JSONTests, ParseNodeEventProductFoundToObject)
{
    std::string data = "{\"Action\":\"ProductFound\",\"Product\":\"Product\",\"Result\":true}";
    std::shared_ptr<FindIt::IMessage> message = parser.Parse(data);
    ASSERT_NE(message, nullptr);
    ASSERT_EQ(message->GetType(), FindIt::MessageType::NODE_EVENT_PRODUCT_FOUND);
    std::shared_ptr<FindIt::NodeEventProductFound> nodeEventProductFound = std::dynamic_pointer_cast<FindIt::NodeEventProductFound>(message);
    ASSERT_EQ(nodeEventProductFound->GetAction(), "ProductFound");
    ASSERT_EQ(nodeEventProductFound->GetProduct(), "Product");
    ASSERT_EQ(nodeEventProductFound->GetResult(), true);
}

TEST_F(JSONTests, ParseNodeEventProductFoundToObjectLowerCaseActionFail)
{
    std::string data = "{\"Action\":\"productfound\",\"Product\":\"Product\",\"Result\":true}";
    std::shared_ptr<FindIt::IMessage> message = parser.Parse(data);
    ASSERT_EQ(message, nullptr);
}

TEST_F(JSONTests, ParseNodeEventProductFoundToObjectMissingProductFail)
{
    std::string data = "{\"Action\":\"ProductFound\",\"Result\":true}";
    std::shared_ptr<FindIt::IMessage> message = parser.Parse(data);
    ASSERT_EQ(message, nullptr);
}

TEST_F(JSONTests, ParseNodeEventProductFoundToObjectMissingResultFail)
{
    std::string data = "{\"Action\":\"ProductFound\",\"Product\":\"Product\"}";
    std::shared_ptr<FindIt::IMessage> message = parser.Parse(data);
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
    ASSERT_EQ(data, "{\"Action\":\"RequestProduct\",\"Activate\":false,\"Product\":\"Product\"}");
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
    std::shared_ptr<FindIt::IMessage> message = parser.Parse(data);
    ASSERT_NE(message, nullptr);
    ASSERT_EQ(message->GetType(), FindIt::MessageType::HEARTBEAT);
}

TEST_F(JSONTests, ParseHeartBeatResponseToJSONToObject)
{
    FindIt::HeartBeatResponse heartBeatResponse("NodeName", 3);
    std::string data = parser.Parse(heartBeatResponse);
    std::shared_ptr<FindIt::IMessage> message = parser.Parse(data);
    ASSERT_NE(message, nullptr);
    ASSERT_EQ(message->GetType(), FindIt::MessageType::HEARTBEAT_RESPONSE);
    std::shared_ptr<FindIt::HeartBeatResponse> heartBeatResponse2 = std::dynamic_pointer_cast<FindIt::HeartBeatResponse>(message);
    ASSERT_EQ(heartBeatResponse2->GetAction(), "HeartBeat");
    ASSERT_EQ(heartBeatResponse2->GetNode(), "NodeName");
    ASSERT_EQ(heartBeatResponse2->GetPlaces(), 3);
}

TEST_F(JSONTests, ParseSignInToJSONToObject)
{
    FindIt::NodeSignIn signIn("NodeName", 3);
    std::string data = parser.Parse(signIn);
    std::shared_ptr<FindIt::IMessage> message = parser.Parse(data);
    ASSERT_NE(message, nullptr);
    ASSERT_EQ(message->GetType(), FindIt::MessageType::NODE_SIGN_IN);
    std::shared_ptr<FindIt::NodeSignIn> signIn2 = std::dynamic_pointer_cast<FindIt::NodeSignIn>(message);
    ASSERT_EQ(signIn2->GetAction(), "SignIn");
    ASSERT_EQ(signIn2->GetNode(), "NodeName");
    ASSERT_EQ(signIn2->GetPlaces(), 3);
}

TEST_F(JSONTests, ParseSignInResponseToJSONToObject)
{
    FindIt::NodeSignInResponse signInResponse("NodeName", 3, true);
    std::string data = parser.Parse(signInResponse);
    std::shared_ptr<FindIt::IMessage> message = parser.Parse(data);
    ASSERT_NE(message, nullptr);
    ASSERT_EQ(message->GetType(), FindIt::MessageType::NODE_SIGN_IN_RESPONSE);
    std::shared_ptr<FindIt::NodeSignInResponse> signInResponse2 = std::dynamic_pointer_cast<FindIt::NodeSignInResponse>(message);
    ASSERT_EQ(signInResponse2->GetAction(), "SignIn");
    ASSERT_EQ(signInResponse2->GetNode(), "NodeName");
    ASSERT_EQ(signInResponse2->GetPlaces(), 3);
    ASSERT_EQ(signInResponse2->GetResult(), true);
}

TEST_F(JSONTests, ParseNodeNotifyNewProductResponseToJSONToObject)
{
    FindIt::NodeNotifyNewProductResponse nodeNotifyNewProductResponse("Product", true);
    std::string data = parser.Parse(nodeNotifyNewProductResponse);
    std::shared_ptr<FindIt::IMessage> message = parser.Parse(data);
    ASSERT_NE(message, nullptr);
    ASSERT_EQ(message->GetType(), FindIt::MessageType::NODE_NOTIFY_NEW_PRODUCT_RESPONSE);
    std::shared_ptr<FindIt::NodeNotifyNewProductResponse> nodeNotifyNewProductResponse2 = std::dynamic_pointer_cast<FindIt::NodeNotifyNewProductResponse>(message);
    ASSERT_EQ(nodeNotifyNewProductResponse2->GetAction(), "NotifyNewProduct");
    ASSERT_EQ(nodeNotifyNewProductResponse2->GetProduct(), "Product");
    ASSERT_EQ(nodeNotifyNewProductResponse2->GetResult(), true);
}

TEST_F(JSONTests, ParseNodeNotifyNewProductToJSONToObject)
{
    FindIt::NodeNotifyNewProduct nodeNotifyNewProduct("Product");
    std::string data = parser.Parse(nodeNotifyNewProduct);
    std::shared_ptr<FindIt::IMessage> message = parser.Parse(data);
    ASSERT_NE(message, nullptr);
    ASSERT_EQ(message->GetType(), FindIt::MessageType::NODE_NOTIFY_NEW_PRODUCT);
    std::shared_ptr<FindIt::NodeNotifyNewProduct> nodeNotifyNewProduct2 = std::dynamic_pointer_cast<FindIt::NodeNotifyNewProduct>(message);
    ASSERT_EQ(nodeNotifyNewProduct2->GetAction(), "NotifyNewProduct");
    ASSERT_EQ(nodeNotifyNewProduct2->GetProduct(), "Product");
}

TEST_F(JSONTests, ParseServerRequestProductToJSONToObject)
{
    FindIt::ServerRequestProduct serverRequestProduct("Product", false);
    std::string data = parser.Parse(serverRequestProduct);
    std::shared_ptr<FindIt::IMessage> message = parser.Parse(data);
    ASSERT_NE(message, nullptr);
    ASSERT_EQ(message->GetType(), FindIt::MessageType::SERVER_REQUEST_PRODUCT);
    std::shared_ptr<FindIt::ServerRequestProduct> serverRequestProduct2 = std::dynamic_pointer_cast<FindIt::ServerRequestProduct>(message);
    ASSERT_EQ(serverRequestProduct2->GetAction(), "RequestProduct");
    ASSERT_EQ(serverRequestProduct2->GetProduct(), "Product");
    ASSERT_EQ(serverRequestProduct2->GetActivate(), false);
}

TEST_F(JSONTests, ParseNodeRespondToProductRequestToJSONToObject)
{
    FindIt::NodeRespondToProductRequest nodeRespondToProductRequest("Product", true);
    std::string data = parser.Parse(nodeRespondToProductRequest);
    std::shared_ptr<FindIt::IMessage> message = parser.Parse(data);
    ASSERT_NE(message, nullptr);
    ASSERT_EQ(message->GetType(), FindIt::MessageType::NODE_RESPOND_TO_PRODUCT_REQUEST);
    std::shared_ptr<FindIt::NodeRespondToProductRequest> nodeRespondToProductRequest2 = std::dynamic_pointer_cast<FindIt::NodeRespondToProductRequest>(message);
    ASSERT_EQ(nodeRespondToProductRequest2->GetAction(), "ResponseProduct");
    ASSERT_EQ(nodeRespondToProductRequest2->GetProduct(), "Product");
    ASSERT_EQ(nodeRespondToProductRequest2->GetResult(), true);
}

TEST_F(JSONTests, ParseNodeEventProductFoundToJSONToObject)
{
    FindIt::NodeEventProductFound nodeEventProductFound("Product", true);
    std::string data = parser.Parse(nodeEventProductFound);
    std::shared_ptr<FindIt::IMessage> message = parser.Parse(data);
    ASSERT_NE(message, nullptr);
    ASSERT_EQ(message->GetType(), FindIt::MessageType::NODE_EVENT_PRODUCT_FOUND);
    std::shared_ptr<FindIt::NodeEventProductFound> nodeEventProductFound2 = std::dynamic_pointer_cast<FindIt::NodeEventProductFound>(message);
    ASSERT_EQ(nodeEventProductFound2->GetAction(), "ProductFound");
    ASSERT_EQ(nodeEventProductFound2->GetProduct(), "Product");
    ASSERT_EQ(nodeEventProductFound2->GetResult(), true);
}

} // namespace
