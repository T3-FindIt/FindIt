#include "Object.hpp"

namespace FindIt
{
Object::Object(std::string Name, int ID)
{
    this->name = Name;
    this->id = ID;
}

Object::~Object()
{
}

std::string Object::GetName()
{
    return this->name;
}

int Object::GetID()
{
    return this->id;
}

bool Object::operator==(const Object &other) const
{
    return this->name == other.name && this->id == other.id;
}

};
