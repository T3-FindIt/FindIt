#include <gtest/gtest.h>

#include "JsonBuilder.hpp"

TEST(JsonBuilderTest, Test_Deserialize_Correct_Message)
{
    JsonBuilder jsonBuilder = JsonBuilder();
    std::string json = "{\"Action\":\"HeartBeat\"}";
    EXPECT_TRUE(jsonBuilder.Deserialize(json));
}

// TEST(JsonBuilderTest, Test_Deserialize_Incorrect_Payload)
// {
//     JsonBuilder jsonBuilder = JsonBuilder();
//     std::string json = "{\"Action\":\"HeatBeater\"}";
//     EXPECT_FALSE(jsonBuilder.Deserialize(json));
// }

// TEST(JsonBuilderTest, Test_Deserialize_Incorrect_Header)
// {
//     JsonBuilder jsonBuilder = JsonBuilder();
//     std::string json = "{\"Act1on\":\"HeartBeat\"}";
//     EXPECT_FALSE(jsonBuilder.Deserialize(json));
// }

// TEST(JsonBuilderTest, Test_Deserialize_Empty_Message)
// {
//     JsonBuilder jsonBuilder = JsonBuilder();
//     std::string json = "";
//     EXPECT_FALSE(jsonBuilder.Deserialize(json));
// }

// TEST(JsonBuilderTest, Test_Serialize_NULL_Action)
// {
//     JsonBuilder jsonBuilder = JsonBuilder();
//     int data[1] = {0};
//     std::string output = "";
//     EXPECT_FALSE(jsonBuilder.Serialize(NULL, data, 1, output));
// }

// TEST(JsonBuilderTest, Test_Serialize_NULL_Data)
// {
//     JsonBuilder jsonBuilder = JsonBuilder();
//     std::string actions[1] = {"HeartBeat"};
//     std::string output = "";
//     EXPECT_FALSE(jsonBuilder.Serialize(actions, NULL, 1, output));
// }

// TEST(JsonBuilderTest, Test_Serialize_Zero_Size)
// {
//     JsonBuilder jsonBuilder = JsonBuilder();
//     std::string actions[1] = {"HeartBeat"};
//     int data[1] = {0};
//     std::string output = "";
//     EXPECT_FALSE(jsonBuilder.Serialize(actions, data, 0, output));
//     EXPECT_FALSE(jsonBuilder.Serialize(actions, data, -1, output));
// }

// TEST(JsonBuilderTest, Test_Serialize_Correct_Message)
// {
//     JsonBuilder jsonBuilder = JsonBuilder();
//     std::string actions[1] = {"HeartBeat"};
//     int data[1] = {0};
//     std::string output = "";
//     EXPECT_TRUE(jsonBuilder.Serialize(actions, data, 1, output));
//     EXPECT_EQ(output, "{\"Action\":\"HeartBeat\",\"Data\":0}");
// }


// TEST(DummyTest, ShouldPass)
// {
//     EXPECT_EQ(1, 1);
// }

// TEST(DummyTest, ShouldFail)
// {
//     EXPECT_EQ(1, 2);
// }

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
    ::testing::InitGoogleTest(&argc, argv);
    if (RUN_ALL_TESTS())
    ;
    return 0;
}
#endif
