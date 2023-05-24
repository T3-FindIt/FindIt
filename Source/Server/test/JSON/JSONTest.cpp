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

} // namespace
