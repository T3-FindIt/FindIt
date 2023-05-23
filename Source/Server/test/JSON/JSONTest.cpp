#include <gtest/gtest.h>
#include <JSONProtocolParser.hpp>

namespace
{

class JSONTests : public ::testing::Test
{
protected:
    FindIt::JSONProtocolParser parser;
};

TEST_F(JSONTests, ParseHeartBeatToObject)
{
    std::string data = "{\"action\":\"HeartBeat\"}";
    FindIt::IMessage* message = parser.Parse(data);
    ASSERT_NE(message, nullptr);
    ASSERT_EQ(message->GetType(), FindIt::MessageType::HEARTBEAT);
    FindIt::HeartBeat* heartbeat = dynamic_cast<FindIt::HeartBeat*>(message);
    ASSERT_EQ(heartbeat->GetAction(), "HeartBeat");
    delete message;
}

TEST_F(JSONTests, ParseSignInToObject)
{
    std::string data = "{\"action\":\"SignIn\",\"Node\":\"NodeName\",\"Places\":3}";
    FindIt::IMessage* message = parser.Parse(data);
    ASSERT_NE(message, nullptr);
    ASSERT_EQ(message->GetType(), FindIt::MessageType::NODE_SIGN_IN);
    FindIt::NodeSignIn* signIn = dynamic_cast<FindIt::NodeSignIn*>(message);
    ASSERT_EQ(signIn->GetAction(), "SignIn");
    ASSERT_EQ(signIn->GetNode(), "NodeName");
    ASSERT_EQ(signIn->GetPlaces(), 3);
    delete message;
}

} // namespace
