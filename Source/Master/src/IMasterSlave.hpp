#ifndef IMASTERSLAVE_HPP
#define IMASTERSLAVE_HPP

enum Node_Registers
{
    RequestForm = 0x0,
    Notification = 0x1, // | SOUND | VIBRATION | LED | RGB | 0 | 0 | 0 | 0 |
    RGB = 0x2, // | R | G | B | 0 | 0 | 0 | 0 | 0 |
    Item = 0x3, // Points to a char[] of size 48.
    Active = 0x4, // 0 = inactive, 1 = active
    Error = 0x5,
};

class IMasterSlave
{
    private:
    
    public:
    void virtual Send(int address,Node_Registers register, int data) = 0;
    bool virtual Recieve(int address,Node_Registers register, int* data) = 0;
    bool virtual Recieve(int address, Node_Registers register, char* data, int size) = 0;
    void virtual Scan(int* addresses, int size) = 0;
};

#endif
