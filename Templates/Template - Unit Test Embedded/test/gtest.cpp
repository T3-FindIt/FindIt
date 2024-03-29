#include <gtest/gtest.h>

TEST(DummyTest, ShouldPass)
{
    EXPECT_EQ(1, 1);
}

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
