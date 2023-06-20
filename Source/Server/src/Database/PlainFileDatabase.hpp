#ifndef PLAINFILEDATABASE_HPP
#define PLAINFILEDATABASE_HPP

#include "IDatabase.hpp"
#include <ItemType.hpp>

#include <string>
#include <mutex>

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
    std::mutex mutex;
private:
    std::vector<std::string> _ReadObjectAndIncrement(std::fstream& file);

    void _Add(ItemType object);
    void _Remove(ItemType object);
    bool _SearchIfPresent(ItemType object);
    std::vector<ItemType> _GetAllObjects();
};

} // namespace FindIt

#endif // PLAINFILEDATABASE_HPP
