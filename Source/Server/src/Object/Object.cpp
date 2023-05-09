#include "Object.hpp"

namespace FindIt
{
Object::Object(std::string Name, int ID)
{
    this->name = Name;
    this->id = ID;
}

std::string Object::GetName()
{
    return this->name;
}

int Object::GetID()
{
    return this->id;
}

};
