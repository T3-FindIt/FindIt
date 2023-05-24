#ifndef PLAINFILEDATABASE_HPP
#define PLAINFILEDATABASE_HPP

#include "IDatabase.hpp"
#include <Object.hpp>

#include <string>

namespace FindIt
{

class PlainFileDatabase : public IDatabase
{
public:
    PlainFileDatabase(std::string relativePath);
    ~PlainFileDatabase() = default;
    void Add(Object object) final;
    void Remove(Object object) final;
    bool SearchIfPresent(Object object) final;
    std::vector<Object> GetAllObjects() final;
private:
    std::string relativePath;
private:
    std::vector<std::string> ReadObjectAndIncrement(std::fstream& file);
};

} // namespace FindIt

#endif // PLAINFILEDATABASE_HPP
