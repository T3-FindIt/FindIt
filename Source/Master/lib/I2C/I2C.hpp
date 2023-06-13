#ifndef I2C_HPP
#define I2C_HPP

#include <Wire.h>
#include "IMasterSlave.hpp"

#define MAX_STRING_SIZE 30
#define LAST_ITEM_REGISTER 3

class I2C : public IMasterSlave
{
    private:
    int address;
    public:


    I2C(int address);
    void Send(int address,int node_Register, int data);
    void Request(int address, int node_Register, int sizeOfData);
    void Scan();
    int GetRegister(int my_register, void* data);
    int GetLastChange();
    int GetNodeCount();
    bool IsAvailable();
    int GetLastAddress();
    void InitializeAddresses();
};

#endif // I2C_HPP
