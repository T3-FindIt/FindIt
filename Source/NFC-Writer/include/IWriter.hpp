#ifndef IWRITER_HPP
#define IWRITER_HPP

class IWriter
{
public:
    virtual ~IWriter() = default;
    virtual void Begin() = 0;
    virtual int Write(char *message) = 0;
};

#endif // IWRITER_HPP
