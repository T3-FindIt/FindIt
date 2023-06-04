#ifndef ITEMTYPE_HPP
#define ITEMTYPE_HPP

#include <string>

namespace FindIt
{

class ItemType
{
public:
    ItemType(const std::string& Name, int ID);
    ~ItemType() = default;

    const std::string& GetName() const;
    int GetID() const;
    bool operator==(const ItemType& other) const;
private:
    std::string name;
    int id;
};

};

#endif // ITEMTYPE_HPP
