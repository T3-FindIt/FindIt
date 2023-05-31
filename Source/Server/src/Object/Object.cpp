#include "Object.hpp"

namespace FindIt
{
Object::Object(std::string Name, int ID) //use consts and references please.
{
    //Absolutely no need for the this calls.
    this->name = Name;
    this->id = ID;
}

Object::~Object() // == default?
{
}

std::string Object::GetName() //consts and references
{
    return this->name;
}

int Object::GetID()
{
    return this->id;
}

bool Object::operator==(const Object &other) const //Nice
{
    return this->name == other.name && this->id == other.id;
}

};
