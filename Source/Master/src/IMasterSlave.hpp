#ifndef IMASTERSLAVE_HPP
#define IMASTERSLAVE_HPP

enum Node_Registers
{
    NR_None = - 0x1,
    NR_RequestForm = 0x0,
    NR_Notification = 0x1, // | SOUND | VIBRATION | LED | RGB | 0 | 0 | 0 | 0 |
    NR_RGB = 0x2, // | R | G | B | 0 | 0 | 0 | 0 | 0 |
    NR_Item = 0x3, // Points to a char[] of size 48.
    NR_Active = 0x4, // 0 = inactive, 1 = active
    NR_Error = 0x5,
};

class IMasterSlave
{
    public:
    void virtual Send(int address,Node_Registers register, int data) = 0;
    void virtual Scan(int* addresses, int size) = 0;
    int virtual GetRegister(Node_Registers my_register, void* data) = 0;
    Node_Registers virtual GetLastChange() = 0;
};

#endif // IMASTERSLAVE_HPP
