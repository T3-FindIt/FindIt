#ifndef IMASTERSLAVE_HPP
#define IMASTERSLAVE_HPP

class IMasterSlave
{
    public:
    void virtual Send(int address, int node_Register, int data) = 0;
    void virtual Scan() = 0;
    int virtual GetRegister(int my_register, void* data) = 0;
    int virtual GetLastChange() = 0;
};

#endif // IMASTERSLAVE_HPP
