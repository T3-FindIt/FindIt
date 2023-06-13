#ifndef ITEMTYPE_HPP
#define ITEMTYPE_HPP

#include <string>

namespace FindIt
{

class ItemType
{
public:
    ItemType(const std::string& Name);
    ~ItemType() = default;

    const std::string& GetName() const;
    bool operator==(const ItemType& other) const;
private:
    std::string name;
};

};

#endif // ITEMTYPE_HPP
