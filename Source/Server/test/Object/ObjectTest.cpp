#include <gtest/gtest.h>
#include <Object.hpp>

namespace
{

TEST(ObjectTests, TestGetName)
{
    FindIt::Object obj("Test", 1);
    ASSERT_EQ(obj.GetName(), "Test");
}

TEST(ObjectTests, TestGetID)
{
    FindIt::Object obj("Test", 1);
    ASSERT_EQ(obj.GetID(), 1);
}

TEST(ObjectTests, TestOperatorEqualWhenEqual)
{
    FindIt::Object obj1("Test", 1);
    FindIt::Object obj2("Test", 1);
    ASSERT_TRUE(obj1 == obj2);
}

TEST(ObjectTests, TestOperatorEqualWhenNotEqualID)
{
    FindIt::Object obj1("Test", 1);
    FindIt::Object obj2("Test", 2);
    ASSERT_FALSE(obj1 == obj2);
}

TEST(ObjectTests, TestOperatorEqualWhenNotEqualName)
{
    FindIt::Object obj1("Test", 1);
    FindIt::Object obj2("Test2", 1);
    ASSERT_FALSE(obj1 == obj2);
}

} // namespace
