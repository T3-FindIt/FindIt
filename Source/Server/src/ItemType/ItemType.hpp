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
    ItemType(const std::string& Name, int ID);
    ~ItemType() = default;

    const std::string& GetName() const;
    const int GetID() const;
    bool operator==(const ItemType& other) const;
};

};

#endif // ITEMTYPE_HPP
