#include "ItemType.hpp"

namespace FindIt
{
ItemType::ItemType(std::string Name, int ID)
{
    this->name = Name;
    this->id = ID;
}

ItemType::~ItemType()
{
}

std::string ItemType::GetName()
{
    return this->name;
}

int ItemType::GetID()
{
    return this->id;
}

bool ItemType::operator==(const ItemType &other) const
{
    return this->name == other.name && this->id == other.id;
}

};
