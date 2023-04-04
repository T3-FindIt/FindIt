#ifndef IMASTERSLAVE_HPP
#define IMASTERSLAVE_HPP

enum Node_Registers
{
    RequestForm = 0x0,
    Ntific
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
