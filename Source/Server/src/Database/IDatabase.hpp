#ifndef IDATABASE_HPP
#define IDATABASE_HPP

#include "../Object/Object.hpp"

#include <vector>

namespace FindIt
{

class IDatabase
{
public:
    virtual ~IDatabase() = default;
    virtual void Add(Object object) = 0;
    virtual void Remove(Object object) = 0;
    virtual void SearchIfPresent(Object object) = 0;
    virtual std::vector<Object> GetAllObjects() = 0;
};

};

#endif // IDATABASE_HPP