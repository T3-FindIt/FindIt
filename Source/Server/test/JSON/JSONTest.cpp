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

} // namespace
