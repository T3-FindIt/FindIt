#ifndef I2C_HPP
#define I2C_HPP

#include <Wire.h>
#include "IMasterSlave.hpp"

class I2C : public IMasterSlave
{
    private:
    int address;
    public:
    I2C(int address);
    void Send(int address,Node_Registers register, int data);
    void Scan(int* addresses, int size);
    int GetRegister(Node_Registers my_register, void* data);
    Node_Registers GetLastChange();
};

#endif
