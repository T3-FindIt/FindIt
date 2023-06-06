#include <gtest/gtest.h>
#include <ItemType.hpp>

namespace
{

TEST(ObjectTests, TestGetName)
{
    FindIt::ItemType obj("Test", 1);
    ASSERT_EQ(obj.GetName(), "Test");
}

TEST(ObjectTests, TestGetID)
{
    FindIt::ItemType obj("Test", 1);
    ASSERT_EQ(obj.GetID(), 1);
}

TEST(ObjectTests, TestOperatorEqualWhenEqual)
{
    FindIt::ItemType obj1("Test", 1);
    FindIt::ItemType obj2("Test", 1);
    ASSERT_TRUE(obj1 == obj2);
}

TEST(ObjectTests, TestOperatorEqualWhenNotEqualID)
{
    FindIt::ItemType obj1("Test", 1);
    FindIt::ItemType obj2("Test", 2);
    ASSERT_FALSE(obj1 == obj2);
}

TEST(ObjectTests, TestOperatorEqualWhenNotEqualName)
{
    FindIt::ItemType obj1("Test", 1);
    FindIt::ItemType obj2("Test2", 1);
    ASSERT_FALSE(obj1 == obj2);
}

} // namespace
