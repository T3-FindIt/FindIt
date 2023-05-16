#include <gtest/gtest.h>
#include "JsonBuilder.hpp"

TEST(JsonBuilderTest, Test_Deserialize_Correct_Message)
{
    JsonBuilder jsonBuilder;
    std::string json = "{\"Action\":\"HeartBeat\"}";
    std::string keys[] = {""};
    std::string values[] = {""};
    int length = 1;

    EXPECT_TRUE(jsonBuilder.Deserialize(json, keys, values, length));
    EXPECT_EQ("Action", keys[0]);
    EXPECT_EQ("HeartBeat", values[0]);
}

TEST(JsonBuilderTest, Test_Deserialize_Correct_Message_Long)
{
    JsonBuilder jsonBuilder;
    std::string json = "{\"Action\": \"HeartBeat\",\"Product\": \"ProductName\",\"Result\": \"True\"}";
    int length = 3;
    std::string keys[3];
    std::string values[3];

    EXPECT_TRUE(jsonBuilder.Deserialize(json, keys, values, length));
    EXPECT_EQ("Action", keys[0]);
    EXPECT_EQ("HeartBeat", values[0]);

    EXPECT_EQ("Product", keys[1]);
    EXPECT_EQ("ProductName", values[1]);

    EXPECT_EQ("Result", keys[2]);
    EXPECT_EQ("True", values[2]);
}

TEST(JsonBuilderTest, Test_Deserialize_Empty_Message)
{
    JsonBuilder jsonBuilder;
    std::string json = "";
    std::string keys[] = {""};
    std::string values[] = {""};
    int length = 1;

    EXPECT_FALSE(jsonBuilder.Deserialize(json, keys, values, length));
}

TEST(JsonBuilderTest, Test_Deserialize_Zero_Size)
{
    JsonBuilder jsonBuilder;
    std::string json = "{\"Action\":\"HeartBeat\"}";
    std::string keys[] = { "" };
    std::string values[] = { "" };
    int length = 0;

    EXPECT_FALSE(jsonBuilder.Deserialize(json, keys, values, length));
}

TEST(JsonBuilderTest, Test_Deserialize_NULL_Keys)
{
    JsonBuilder jsonBuilder;
    std::string json = "{\"Action\":\"HeartBeat\"}";
    std::string values[] = {""};
    int length = 1;

    EXPECT_FALSE(jsonBuilder.Deserialize(json, nullptr, values, length));
}

TEST(JsonBuilderTest, Test_Deserialize_NULL_Values)
{
    JsonBuilder jsonBuilder;
    std::string json = "{\"Action\":\"HeartBeat\"}";
    std::string keys[] = {""};
    int length = 1;

    EXPECT_FALSE(jsonBuilder.Deserialize(json, keys, nullptr, length));
}

TEST(JsonBuilderTest, Test_Serialize_NULL_Action)
{
    JsonBuilder jsonBuilder;
    std::string json = "";
    std::string values[] = {""};
    int length = 1;

    EXPECT_FALSE(jsonBuilder.Deserialize(json, nullptr, values, length));
}

TEST(JsonBuilderTest, Test_Serialize_NULL_Data)
{
    JsonBuilder jsonBuilder;
    std::string json = "";
    std::string actions[1] = {"HeartBeat"};
    std::string output = "";
    EXPECT_FALSE(jsonBuilder.Serialize(actions, NULL, 1, output));
}

TEST(JsonBuilderTest, Test_Serialize_Zero_Size)
{
    JsonBuilder jsonBuilder;
    std::string actions[1] = {"HeartBeat"};
    std::string data[] = {"0"};
    std::string output = "";
    EXPECT_FALSE(jsonBuilder.Serialize(actions, data, 0, output));
    EXPECT_FALSE(jsonBuilder.Serialize(actions, data, - 1, output));
}

TEST(JsonBuilderTest, Test_Serialize_Correct_Message)
{
    JsonBuilder jsonBuilder;
    std::string json = "";
    std::string keys[] = {"Action", "Data"};
    std::string data[] = {"HeartBeat", "0"};
    int size = 2;
    EXPECT_TRUE(jsonBuilder.Serialize(keys, data, size, json));
    EXPECT_EQ(json, "{\"Action\":\"HeartBeat\",\"Data\":\"0\"}");
}


#if defined(ARDUINO)
#include <Arduino.h>

void setup()
{
    // should be the same value as for the `test_speed` option in "platformio.ini"
    // default value is test_speed=115200
    Serial.begin(115200);

    ::testing::InitGoogleTest();
}

void loop()
{
    // Run tests
    if (RUN_ALL_TESTS())
    ;

    // sleep 1 sec
    delay(1000);
}

#else
int main(int argc, char **argv)
{
    JsonBuilder builder;
    ::testing::InitGoogleTest(&argc, argv);
    if (RUN_ALL_TESTS())
    ;
    return 0;
}
#endif
