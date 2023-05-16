#ifndef IREADER_HPP
#define IREADER_HPP

class IReader
{
private:
public:
    IReader(){}
    virtual ~IReader(){}
    virtual bool CheckForCard() = 0;
    virtual int ReadCard(char* outputString) = 0; 
};

#endif // IREADER_HPP
