#include "ItemType.hpp"

namespace FindIt
{

ItemType::ItemType(const std::string& Name)
    : name(Name)
{
}

const std::string& ItemType::GetName() const
{
    return name;
}

bool ItemType::operator==(const ItemType &other) const
{
    return name == other.name;
}

};
