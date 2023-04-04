#ifndef I2C_HPP
#define I2C_HPP

#include <Wire.h>

class I2C : public IMasterSlave
{
    private:
    int address;
    public:
    I2C(int address);
    void Send(int address,Node_Registers register, int data);
    bool Recieve(int address,Node_Registers register, int* data);
    bool Recieve(int address, Node_Registers register, char* data, int size);
    void Scan(int* addresses, int size);
};

#endif