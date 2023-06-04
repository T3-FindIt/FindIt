#ifndef ITEMTYPE_HPP
#define ITEMTYPE_HPP

#include <string>

namespace FindIt
{

class ItemType
{
private:
    std::string name;
    int id;
public:
    ItemType(std::string name, int id);
    ~ItemType();

    std::string GetName();
    int GetID();
    bool operator==(const ItemType& other) const;
};

};

#endif // ITEMTYPE_HPP
