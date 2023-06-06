#include "ItemType.hpp"

namespace FindIt
{

ItemType::ItemType(const std::string& Name, int ID)
    : name(Name), id(ID)
{
}

const std::string& ItemType::GetName() const
{
    return name;
}

int ItemType::GetID() const
{
    return id;
}

bool ItemType::operator==(const ItemType &other) const
{
    return name == other.name && id == other.id;
}

};
