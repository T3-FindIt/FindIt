#ifndef PLAINFILEDATABASE_HPP
#define PLAINFILEDATABASE_HPP

#include "IDatabase.hpp"
#include <ItemType.hpp>

#include <string>

namespace FindIt
{

class PlainFileDatabase : public IDatabase
{
public:
    PlainFileDatabase(std::string relativePath);
    ~PlainFileDatabase() = default;
    void Add(ItemType object) final;
    void Remove(ItemType object) final;
    bool SearchIfPresent(ItemType object) final;
    std::vector<ItemType> GetAllObjects() final;
private:
    std::string relativePath;
private:
    std::vector<std::string> ReadObjectAndIncrement(std::fstream& file);
};

} // namespace FindIt

#endif // PLAINFILEDATABASE_HPP
