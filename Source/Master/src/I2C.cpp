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
    Wire.onReceive(receiveEvent);
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
                break; // Do the thing
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

int GetRegister(Node_Registers my_register, void* data)
{
    switch (my_register)
    {
        case Node_Registers::Notification:
        {
            return Notification;
        }
        case Node_Registers::RGB:
        {
            return RGB;
        }
        case Node_Registers::Item:
        {
            strcpy((char*)data, Item);
            return 0;
        }
        case Node_Registers::Active:
        {
            return Active;
        }
        case Node_Registers::Error:
        {
            strcpy((char*)data, Error);
            return 0;
        }
        default:
        {
            throw "Invalid Register!"; // This should never happen
        }
    }
}
