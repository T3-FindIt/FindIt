#include <gtest/gtest.h>
#include <PlainFileDatabase.hpp>
#include <ItemType.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace
{

class DatabaseTests : public ::testing::Test
{
protected:
    FindIt::PlainFileDatabase database = FindIt::PlainFileDatabase("DatabaseTestFiles/database.db");
    std::filesystem::path path = std::filesystem::current_path() / "DatabaseTestFiles" / "database.db";

    void TearDown() override
    {
        std::filesystem::remove(path);

        if (path.parent_path() != std::filesystem::current_path())
            std::filesystem::remove(path.parent_path());
    }
};

TEST_F(DatabaseTests, CheckForCorrectFileInit)
{
    ASSERT_TRUE(std::filesystem::exists(path));
}

TEST_F(DatabaseTests, CheckIfNewlyCreatedFileIsEmpty)
{
    std::fstream file(path);
    ASSERT_TRUE(file.peek() == std::fstream::traits_type::eof());
    file.close();
}

TEST_F(DatabaseTests, CheckIfFileIsNotEmptyAfterAddingObject)
{
    FindIt::ItemType object = FindIt::ItemType("TestObject", 20);
    database.Add(object);
    std::fstream file(path);
    ASSERT_FALSE(file.peek() == std::fstream::traits_type::eof());
    file.close();
}

TEST_F(DatabaseTests, CheckForCorrectlyWrittenObject)
{
    FindIt::ItemType object = FindIt::ItemType("TestObject", 20);
    database.Add(object);
    std::fstream file(path);
    std::string line;
    std::getline(file, line);
    ASSERT_EQ(line, "####;TestObject;20;####;");
    file.close();
}

TEST_F(DatabaseTests, CheckForCorrectlyWrittenObjectTwice)
{
    FindIt::ItemType objectA = FindIt::ItemType("TestObject", 20);
    database.Add(objectA);
    FindIt::ItemType objectB = FindIt::ItemType("TestObjectTwo", 34);
    database.Add(objectB);
    std::fstream file(path);
    std::string line;
    std::getline(file, line);
    ASSERT_EQ(line, "####;TestObject;20;####;####;TestObjectTwo;34;####;");
    file.close();
}

TEST_F(DatabaseTests, CheckForSameObjectOnlyWrittenOnceAddedTwice)
{
    FindIt::ItemType object = FindIt::ItemType("TestObject", 20);
    database.Add(object);
    database.Add(object);
    std::fstream file(path);
    std::string line;
    std::getline(file, line);
    ASSERT_EQ(line, "####;TestObject;20;####;");
    file.close();
}

TEST_F(DatabaseTests, RemoveObjectThatIsNotPresent)
{
    FindIt::ItemType objectA = FindIt::ItemType("TestObject", 20);
    database.Add(objectA);
    FindIt::ItemType objectB = FindIt::ItemType("TestObjectTwo", 34);
    database.Remove(objectB);
    std::fstream file(path);
    std::string line;
    std::getline(file, line);
    ASSERT_EQ(line, "####;TestObject;20;####;");
    file.close();
}

TEST_F(DatabaseTests, RemoveObjectFromEmptyDatabase)
{
    FindIt::ItemType object = FindIt::ItemType("TestObject", 20);
    database.Remove(object);
    std::fstream file(path);
    ASSERT_TRUE(file.peek() == std::fstream::traits_type::eof());
    file.close();
}

TEST_F(DatabaseTests, CheckIfObjectIsRemoved)
{
    FindIt::ItemType object = FindIt::ItemType("TestObject", 20);
    database.Add(object);
    std::fstream file(path);
    std::string line;
    std::getline(file, line);
    ASSERT_EQ(line, "####;TestObject;20;####;");
    file.close();
    database.Remove(object);
    file.open(path);
    ASSERT_TRUE(file.peek() == std::fstream::traits_type::eof());
    file.close();
}

TEST_F(DatabaseTests, CheckIfMiddleObjectIsCorrectlyRemoved)
{
    FindIt::ItemType objectA = FindIt::ItemType("TestObject", 20);
    database.Add(objectA);
    FindIt::ItemType objectB = FindIt::ItemType("TestObjectTwo", 34);
    database.Add(objectB);
    FindIt::ItemType objectC = FindIt::ItemType("TestObjectThree", 45);
    database.Add(objectC);
    std::fstream file(path);
    std::string line;
    std::getline(file, line);
    ASSERT_EQ(line, "####;TestObject;20;####;####;TestObjectTwo;34;####;####;TestObjectThree;45;####;");
    file.close();
    database.Remove(objectB);
    file.open(path);
    std::getline(file, line);
    ASSERT_EQ(line, "####;TestObject;20;####;####;TestObjectThree;45;####;");
    file.close();
}

TEST_F(DatabaseTests, GetListOfObjectsFromEmptyDatabase)
{
    std::vector<FindIt::ItemType> objects = database.GetAllObjects();
    ASSERT_TRUE(objects.empty());
}

TEST_F(DatabaseTests, GetListOfObjectsFromDatabaseWithOneObject)
{
    FindIt::ItemType object = FindIt::ItemType("TestObject", 20);
    database.Add(object);
    std::vector<FindIt::ItemType> objects = database.GetAllObjects();
    ASSERT_EQ(objects.size(), 1);
    ASSERT_EQ(objects[0].GetName(), "TestObject");
    ASSERT_EQ(objects[0].GetID(), 20);
}

TEST_F(DatabaseTests, GetListOfObjectsFromDatabaseWithTwoObjects)
{
    FindIt::ItemType objectA = FindIt::ItemType("TestObject", 20);
    database.Add(objectA);
    FindIt::ItemType objectB = FindIt::ItemType("TestObjectTwo", 34);
    database.Add(objectB);
    std::vector<FindIt::ItemType> objects = database.GetAllObjects();
    ASSERT_EQ(objects.size(), 2);
    ASSERT_EQ(objects[0].GetName(), "TestObject");
    ASSERT_EQ(objects[0].GetID(), 20);
    ASSERT_EQ(objects[1].GetName(), "TestObjectTwo");
    ASSERT_EQ(objects[1].GetID(), 34);
}

TEST_F(DatabaseTests, SearchIfNoneExistingObjectIsPresentInEmptyDatabase)
{
    FindIt::ItemType objectToSearch = FindIt::ItemType("TestObjectTwo", 34);
    ASSERT_FALSE(database.SearchIfPresent(objectToSearch));
}

TEST_F(DatabaseTests, SearchIfNoneExistingObjectIsPresentInNotEmptyDatabase)
{
    FindIt::ItemType objectA = FindIt::ItemType("TestObject", 20);
    database.Add(objectA);
    FindIt::ItemType objectToSearch = FindIt::ItemType("TestObjectTwo", 34);
    ASSERT_FALSE(database.SearchIfPresent(objectToSearch));
}

TEST_F(DatabaseTests, SearchIfExistingObjectIsPresentInDatabase)
{
    FindIt::ItemType objectA = FindIt::ItemType("TestObject", 20);
    database.Add(objectA);
    ASSERT_TRUE(database.SearchIfPresent(objectA));
}

TEST_F(DatabaseTests, SearchIfExistingObjectIsPresentInDatabaseWithMultipleElements)
{
    FindIt::ItemType objectA = FindIt::ItemType("TestObject", 20);
    database.Add(objectA);
    FindIt::ItemType objectB = FindIt::ItemType("TestObjectTwo", 34);
    database.Add(objectB);
    FindIt::ItemType objectC = FindIt::ItemType("TestObjectThree", 45);
    database.Add(objectC);
    ASSERT_TRUE(database.SearchIfPresent(objectB));
}

TEST_F(DatabaseTests, IfDatabaseFileGetsDeletedExpectExceptionGetAllObjects)
{
    std::filesystem::remove(path);
    // It is possible that your IDE gives the error:
    // <typeinfo> must be included before typeid is usedC/C++(693)
    // This is a bug in the IDE and can be ignored.
    ASSERT_THROW(database.GetAllObjects(), std::runtime_error);
}

TEST_F(DatabaseTests, IfDatabaseFileGetsDeletedExpectExceptionAdd)
{
    std::filesystem::remove(path);
    FindIt::ItemType object = FindIt::ItemType("TestObject", 20);
    // It is possible that your IDE gives the error:
    // <typeinfo> must be included before typeid is usedC/C++(693)
    // This is a bug in the IDE and can be ignored.
    ASSERT_THROW(database.Add(object), std::runtime_error);
}

TEST_F(DatabaseTests, IfDatabaseFileGetsDeletedExpectExceptionRemove)
{
    std::filesystem::remove(path);
    FindIt::ItemType object = FindIt::ItemType("TestObject", 20);
    // It is possible that your IDE gives the error:
    // <typeinfo> must be included before typeid is usedC/C++(693)
    // This is a bug in the IDE and can be ignored.
    ASSERT_THROW(database.Remove(object), std::runtime_error);
}

TEST_F(DatabaseTests, IfDatabaseFileGetsDeletedExpectExceptionSearchIfPresent)
{
    std::filesystem::remove(path);
    FindIt::ItemType object = FindIt::ItemType("TestObject", 20);
    // It is possible that your IDE gives the error:
    // <typeinfo> must be included before typeid is usedC/C++(693)
    // This is a bug in the IDE and can be ignored.
    ASSERT_THROW(database.SearchIfPresent(object), std::runtime_error);
}

} // namespace
