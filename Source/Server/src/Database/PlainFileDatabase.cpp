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

void PlainFileDatabase::Add(Object object)
{
    if (!SearchIfPresent(object))
    {
        std::fstream file(relativePath, std::ios::app);
        if (!file.good())
        {
            throw std::runtime_error("Database Source Missing - Add");
        }
        file << separator << ';' << object.GetName() << ";" << object.GetID() << ";" << separator << ';';
        file.close();
    }
}

void PlainFileDatabase::Remove(Object object)
{
    std::vector<Object> objects = GetAllObjects();
    std::erase(objects, object);

    std::ofstream file(relativePath, std::ios::trunc);
    file.close();

    for (const auto& obj : objects)
    {
        Add(obj);
    }
}

bool PlainFileDatabase::SearchIfPresent(Object object)
{
    std::fstream file(relativePath);
    if (!file.good())
    {
        throw std::runtime_error("Database Source Missing - SearchIfPresent");
    }
    bool result = false;

    std::vector<std::string> lines;
    lines = ReadObjectAndIncrement(file);
    while (!lines.empty() && lines.size() >= 2)
    {
        if (lines.at(0) == object.GetName() && lines.at(1) == std::to_string(object.GetID()))
        {
            result = true;
            break;
        }
        lines = ReadObjectAndIncrement(file);
    }

    return result;
}

std::vector<Object> PlainFileDatabase::GetAllObjects()
{
    std::fstream file(relativePath);
    if (!file.good())
    {
        throw std::runtime_error("Database Source Missing - GetAllObjects");
    }

    std::vector<Object> objects;
    std::vector<std::string> lines;
    lines = ReadObjectAndIncrement(file);
    while (!lines.empty() && lines.size() >= 2)
    {
        objects.emplace_back(lines.at(0), std::stoi(lines.at(1)));
        lines = ReadObjectAndIncrement(file);
    }

    file.close();
    return objects;
}

std::vector<std::string> PlainFileDatabase::ReadObjectAndIncrement(std::fstream &file)
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

} // namespace FindIt
