#include "I2C.hpp"

#include <string>

#define MAX_STRING_SIZE 48

Node_Registers incomingRegister = Node_Registers::RequestForm;

// ===================== //
// ===== REGISTER ===== //
// ===================== //

int Notification = 0x32; // Enable LED 
int RGB;
char Item[MAX_STRING_SIZE];
int Active;
char Error[MAX_STRING_SIZE];

// ===================== //
// ===== REGISTER ===== //
// ===================== //


I2C::I2C(int address)
{
    this->address = address;
}

void requestEvent() // A node makes a request
{
    // Need to discuss how this is handled
}

void receiveEvent(int howMany) // A node sends data, not making a request.
{
    std::string data = "";
    while (Wire.available())
    {
        data += (char)Wire.read();
    }
    
    if (data.length() > 0)
    {
        switch (incomingRegister)
        {
            case Node_Registers::Item:
            {
                strcpy(Item, data.c_str());
                break;
            }
            case Node_Registers::Error:
            {
                strcpy(Error, data.c_str());
                break;
            }
            default:
            {
                throw "Invalid Register!"; // This should never happen
            }
        }
    }
}

void receiveEvent(int howMany) // A node sends data, not making a request.
{
    while (Wire.available())
    {
        char c = Wire.read();
    }
}

void I2C::Send(int address,Node_Registers nodeRegister, int data)
{
    Wire.beginTransmission(address);
    Wire.write((int)nodeRegister); // What Register to write to // 1 Byte
    Wire.write(data); // What Register to write to // 1 Byte
    Wire.endTransmission();
}

// bool I2C::Recieve(int address,Node_Registers nodeRegister, int* data)
// {
//     Wire.beginTransmission(address);
//     Wire.write(nodeRegister);
//     Wire.endTransmission();
//     Wire.requestFrom(address, 1);
//     if (Wire.available())
//     {
//         *data = Wire.read();
//         return true;
//     }
//     return false;
// }

// bool I2C::Recieve(int address, Node_Registers nodeRegister, char* data, int size)
// {
//     Wire.beginTransmission(address);
//     Wire.write(nodeRegister);
//     Wire.endTransmission();
//     Wire.requestFrom(address, size);
//     if (Wire.available())
//     {
//         for (int i = 0; i < size; i++)
//         {
//             data[i] = Wire.read();
//         }
//         return true;
//     }
//     return false;
// }

void I2C::Scan(int* addresses, int size)
{
    int address = 0;
    int index = 0;
    for (address = 1; address < 127; address++)
    {
        Wire.beginTransmission(address);
        if (Wire.endTransmission() == 0)
        {
            addresses[index] = address;
            index++;
            if (index >= size)
            {
                break;
            }
        }
    }
}
