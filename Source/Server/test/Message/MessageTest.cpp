#include <gtest/gtest.h>
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

namespace
{

//========================================================//
//                                                        //
//                    Client -> Server                    //
//                                                        //
//========================================================//

TEST(MessageTests_ClientToServer, HeartbeatResponseTest)
{
    FindIt::HeartBeatResponse message("node_name", 1);
    EXPECT_EQ(message.GetType(), FindIt::MessageType::HEARTBEAT_RESPONSE);
    EXPECT_EQ(message.GetAction(), "HeartBeat");
    EXPECT_EQ(message.GetNode(), "node_name");
    EXPECT_EQ(message.GetPlaces(), 1);
}

TEST(MessageTests_ClientToServer, NodeEventProductFoundTest)
{
    FindIt::NodeEventProductFound message("product_name", true);
    EXPECT_EQ(message.GetType(), FindIt::MessageType::NODE_EVENT_PRODUCT_FOUND);
    EXPECT_EQ(message.GetAction(), "ProductFound");
    EXPECT_EQ(message.GetProduct(), "product_name");
    EXPECT_EQ(message.GetResult(), true);
}

TEST(MessageTests_ClientToServer, NodeNotifyNewProductTest)
{
    FindIt::NodeNotifyNewProduct message("product_name");
    EXPECT_EQ(message.GetType(), FindIt::MessageType::NODE_NOTIFY_NEW_PRODUCT);
    EXPECT_EQ(message.GetAction(), "NotifyNewProduct");
    EXPECT_EQ(message.GetProduct(), "product_name");
}

TEST(MessageTests_ClientToServer, NodeRespondToProductRequestTest)
{
    FindIt::NodeRespondToProductRequest message("product_name", true);
    EXPECT_EQ(message.GetType(), FindIt::MessageType::NODE_RESPOND_TO_PRODUCT_REQUEST);
    EXPECT_EQ(message.GetAction(), "ResponseProduct");
    EXPECT_EQ(message.GetProduct(), "product_name");
    EXPECT_EQ(message.GetResult(), true);
}

TEST(MessageTests_ClientToServer, NodeSignInTest)
{
    FindIt::NodeSignIn message("node_name", 1);
    EXPECT_EQ(message.GetType(), FindIt::MessageType::NODE_SIGN_IN);
    EXPECT_EQ(message.GetAction(), "SignIn");
    EXPECT_EQ(message.GetNode(), "node_name");
    EXPECT_EQ(message.GetPlaces(), 1);
}

//========================================================//
//                                                        //
//                    Server -> Client                    //
//                                                        //
//========================================================//

TEST(MessageTests_ServerToClient, HeartBeatTest)
{
    FindIt::HeartBeat message;
    EXPECT_EQ(message.GetType(), FindIt::MessageType::HEARTBEAT);
    EXPECT_EQ(message.GetAction(), "HeartBeat");
}

TEST(MessageTests_ServerToClient, NodeNotifyNewProductResponseTest)
{
    FindIt::NodeNotifyNewProductResponse message("product_name", true);
    EXPECT_EQ(message.GetType(), FindIt::MessageType::NODE_NOTIFY_NEW_PRODUCT_RESPONSE);
    EXPECT_EQ(message.GetAction(), "NotifyNewProduct");
    EXPECT_EQ(message.GetProduct(), "product_name");
    EXPECT_EQ(message.GetResult(), true);
}

TEST(MessageTests_ServerToClient, NodeSignInResponseTest)
{
    FindIt::NodeSignInResponse message("node_name", 1, true);
    EXPECT_EQ(message.GetType(), FindIt::MessageType::NODE_SIGN_IN_RESPONSE);
    EXPECT_EQ(message.GetAction(), "SignIn");
    EXPECT_EQ(message.GetNode(), "node_name");
    EXPECT_EQ(message.GetPlaces(), 1);
    EXPECT_EQ(message.GetResult(), true);
}

TEST(MessageTests_ServerToClient, ServerRequestProductTest)
{
    FindIt::ServerRequestProduct message("product_name", true);
    EXPECT_EQ(message.GetType(), FindIt::MessageType::SERVER_REQUEST_PRODUCT);
    EXPECT_EQ(message.GetAction(), "RequestProduct");
    EXPECT_EQ(message.GetProduct(), "product_name");
    EXPECT_EQ(message.GetActivate(), true);
}

//========================================================//
//                                                        //
//                      Polymorphism                      //
//                                                        //
//========================================================//

TEST(MessageTests, PolymorphismTest)
{
    FindIt::IMessage* message = new FindIt::HeartBeatResponse("node_name", 1);
    ASSERT_EQ(message->GetType(), FindIt::MessageType::HEARTBEAT_RESPONSE);
    delete message;

    message = new FindIt::NodeEventProductFound("product_name", true);
    ASSERT_EQ(message->GetType(), FindIt::MessageType::NODE_EVENT_PRODUCT_FOUND);
    delete message;
}

} // namespace
