#ifndef IDATABASE_HPP
#define IDATABASE_HPP

#include <ItemType.hpp>

#include <vector>

namespace FindIt
{

class IDatabase
{
public:
    virtual ~IDatabase() = default;
    virtual void Add(ItemType object) = 0;
    virtual void Remove(ItemType object) = 0;
    virtual bool SearchIfPresent(ItemType object) = 0;
    virtual std::vector<ItemType> GetAllObjects() = 0;
};

} // namespace FindIt

#endif // IDATABASE_HPP
