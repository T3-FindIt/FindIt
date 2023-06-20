#include <gtest/gtest.h>
#include <ItemType.hpp>

namespace
{

TEST(ObjectTests, TestGetName)
{
    FindIt::ItemType obj("Test");
    ASSERT_EQ(obj.GetName(), "Test");
}

TEST(ObjectTests, TestOperatorEqualWhenEqual)
{
    FindIt::ItemType obj1("Test");
    FindIt::ItemType obj2("Test");
    ASSERT_TRUE(obj1 == obj2);
}

TEST(ObjectTests, TestOperatorEqualWhenNotEqualName)
{
    FindIt::ItemType obj1("Test");
    FindIt::ItemType obj2("Test2");
    ASSERT_FALSE(obj1 == obj2);
}

} // namespace
