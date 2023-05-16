#ifndef IREADER_HPP
#define IREADER_HPP

#include <string>

class IReader
{
private:
public:
    IReader(){}
    virtual ~IReader(){}
    virtual bool CheckForCard() = 0;
    virtual std::string ReadCard() = 0; 
};

#endif // IREADER_HPP
