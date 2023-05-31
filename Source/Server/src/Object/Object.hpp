#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <string>

namespace FindIt
{

class Object //Would rename, object is a concept in code and this is confusing
{
private:
    std::string name;
    int id;
public:
    Object(std::string name, int id);
    ~Object();

    std::string GetName();
    int GetID();
    bool operator==(const Object& other) const;
};

};

#endif // OBJECT_HPP
