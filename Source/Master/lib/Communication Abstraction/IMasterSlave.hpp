#ifndef IMASTERSLAVE_HPP
#define IMASTERSLAVE_HPP

// enum Node_Registers
// {
//     NR_None = 255,
//     NR_RequestForm = 0,
//     NR_Notification = 1, // | SOUND | VIBRATION | LED | RGB | 0 | 0 | 0 | 0 |
//     NR_RGB = 2, // | R | G | B | 0 | 0 | 0 | 0 | 0 |
//     NR_Item = 3, // Points to a char[] of size 48.
//     NR_Active = 4, // 0 = inactive, 1 = active
//     NR_Error = 5,
// };

class IMasterSlave
{
    public:
    void virtual Send(int address, int node_Register, int data) = 0;
    void virtual Scan() = 0;
    int virtual GetRegister(int my_register, void* data) = 0;
    int virtual GetLastChange() = 0;
};

#endif // IMASTERSLAVE_HPP
