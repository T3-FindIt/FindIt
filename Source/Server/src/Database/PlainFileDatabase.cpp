#include "PlainFileDatabase.hpp"
#include <fstream>
#include <filesystem>

namespace FindIt
{

const std::string separator = "####";

PlainFileDatabase::PlainFileDatabase(std::string relativePath)
    : relativePath(relativePath)
{
    if (std::string tempPath = relativePath; tempPath.find_last_of('/') != std::string::npos)
    {
        tempPath = tempPath.substr(0, tempPath.find_last_of('/') + 1);

        std::filesystem::path path(tempPath);
        if (!std::filesystem::exists(path))
        {
            std::filesystem::create_directories(path);
        }
    }

    std::fstream file(relativePath);
    if (!file.good())
    {
        std::ofstream createFile(relativePath);
        createFile.close();
    }
}

void PlainFileDatabase::Add(ItemType object)
{
    std::scoped_lock<std::mutex> lock(mutex);
    _Add(object);
}

void PlainFileDatabase::Remove(ItemType object)
{
    std::scoped_lock<std::mutex> lock(mutex);
    _Remove(object);
}

bool PlainFileDatabase::SearchIfPresent(ItemType object)
{
    std::scoped_lock<std::mutex> lock(mutex);
    return _SearchIfPresent(object);
}

std::vector<ItemType> PlainFileDatabase::GetAllObjects()
{
    std::scoped_lock<std::mutex> lock(mutex);
    return _GetAllObjects();
}

// Only used internally, no mutex needed.
std::vector<std::string> PlainFileDatabase::_ReadObjectAndIncrement(std::fstream &file)
{
    if (!file.good())
    {
        return std::vector<std::string>();
    }

    std::vector<std::string> lines;
    std::string line = "";
    bool firstSeparatorFound = false;

    while (file.peek() != EOF)
    {
        char c = (char)file.get();
        if (c == ';')
        {
            if (line == separator && !firstSeparatorFound)
            {
                firstSeparatorFound = true;
                line = "";
            }
            else if (line == separator && firstSeparatorFound)
            {
                break;
            }
            else if (line.size() > 0)
            {
                lines.push_back(line);
                line = "";
            }
        }
        else
        {
            line += c;
        }
    }

    return lines;
}

// Only used internally, no mutex needed.
void PlainFileDatabase::_Add(ItemType object)
{
    if (!_SearchIfPresent(object))
    {
        std::fstream file(relativePath, std::ios::app);
        if (!file.good())
        {
            throw std::runtime_error("Database Source Missing - Add");
        }
        // This is VERY bad, but there is not enough time to change the database format.
        // The ID will always be 1 and never be checked.
        // But trying to remove this caused too much time to be wasted.
        file << separator << ';' << object.GetName() << ";" << '1' << ";" << separator << ';';
        file.close();
    }
}

// Only used internally, no mutex needed.
void PlainFileDatabase::_Remove(ItemType object)
{
    std::vector<ItemType> objects = _GetAllObjects();
    std::erase(objects, object);

    std::ofstream file(relativePath, std::ios::trunc);
    file.close();

    for (const auto& obj : objects)
    {
        _Add(obj);
    }
}

// Only used internally, no mutex needed.
bool PlainFileDatabase::_SearchIfPresent(ItemType object)
{
    std::fstream file(relativePath);
    if (!file.good())
    {
        throw std::runtime_error("Database Source Missing - SearchIfPresent");
    }
    bool result = false;

    std::vector<std::string> lines;
    lines = _ReadObjectAndIncrement(file);
    while (!lines.empty() && lines.size() >= 2)
    {
        if (lines.at(0) == object.GetName())
        {
            result = true;
            break;
        }
        lines = _ReadObjectAndIncrement(file);
    }
    return result;
}

// Only used internally, no mutex needed.
std::vector<ItemType> PlainFileDatabase::_GetAllObjects()
{
    std::fstream file(relativePath);
    if (!file.good())
    {
        throw std::runtime_error("Database Source Missing - GetAllObjects");
    }

    std::vector<ItemType> objects;
    std::vector<std::string> lines;
    lines = _ReadObjectAndIncrement(file);
    while (!lines.empty() && lines.size() >= 2)
    {
        objects.emplace_back(lines.at(0));
        lines = _ReadObjectAndIncrement(file);
    }

    file.close();
    return objects;
}

} // namespace FindIt
