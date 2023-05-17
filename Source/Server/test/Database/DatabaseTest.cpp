#include <gtest/gtest.h>
#include <PlainFileDatabase.hpp>
#include <Object.hpp>

namespace
{

class DatabaseTests : public ::testing::Test
{
protected:
    FindIt::PlainFileDatabase* database;

    virtual void SetUp()
    {
        // database = PlainFileDatabase("test.db");
        std::string relativePath = "test.db";
        database = new FindIt::PlainFileDatabase(relativePath);
    }
};

TEST_F(DatabaseTests, test)
{
    // database.Add(Object("test", 1));
}

} // namespace
