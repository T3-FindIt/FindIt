#ifndef IDATABASE_HPP
#define IDATABASE_HPP

#include <Object.hpp>

#include <vector>

namespace FindIt
{

class IDatabase
{
public:
    virtual ~IDatabase() = default;
    virtual void Add(Object object) = 0;
    virtual void Remove(Object object) = 0;
    virtual bool SearchIfPresent(Object object) = 0;
    virtual std::vector<Object> GetAllObjects() = 0;
};

} // namespace FindIt

#endif // IDATABASE_HPP
