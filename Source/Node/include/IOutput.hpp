#ifndef IOUTPUT_HPP
#define IOUTPUT_HPP

class IOutput
{
public:
    IOutput(){}
    virtual ~IOutput() = default;
    virtual int On() = 0;
    virtual int Off() = 0;
};

#endif // IOUTPUT_HPP
