#ifndef I2C_HPP
#define I2C_HPP

#include <Wire.h>
#include "IMasterSlave.hpp"

#define MAX_STRING_SIZE 30

class I2C : public IMasterSlave
{
    private:
    int address;
    public:


    I2C(int address);
    void Send(int address,Node_Registers node_Register, int data);
    void Request(int address, Node_Registers node_Register, int sizeOfData);
    void Scan();
    int GetRegister(Node_Registers my_register, void* data);
    Node_Registers GetLastChange();

    int debug_GetLastAddress();
};

#endif // I2C_HPP
