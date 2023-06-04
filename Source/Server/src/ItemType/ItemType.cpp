#include "ItemType.hpp"

namespace FindIt
{

ItemType::ItemType(const std::string& Name, int ID)
    : name(Name), id(ID)
{
}

const std::string& ItemType::GetName() const
{
    return this->name;
}

const int ItemType::GetID() const
{
    return this->id;
}

bool ItemType::operator==(const ItemType &other) const
{
    return this->name == other.name && this->id == other.id;
}

};
