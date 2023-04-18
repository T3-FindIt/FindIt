#include "I2C.hpp"

#include <string>

#define MAX_STRING_SIZE 48

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

Node_Registers incomingRegister = Node_Registers::NR_None;
Node_Registers lastRecieved;

void receiveEvent(int howMany) // A node sends data, not making a request.
{
    std::string data = "";
    while (Wire.available())
    {
        data += (char)Wire.read();
    }
    
    if (data.length() > 1)
    {
        switch (incomingRegister)
        {
            case Node_Registers::NR_Item:
            {
                strcpy(Item, data.c_str());
                break;
            }
            case Node_Registers::NR_Error:
            {
                break; // Do the thing
            }
            default:
            {
                throw "Invalid Register!"; // This should never happen
            }
        }
        lastRecieved = incomingRegister;
        incomingRegister = Node_Registers::NR_None;

        return;
    }
    else if(data.length() == 1)
    {
        incomingRegister = (Node_Registers)data[0];
        return;
    }

}

I2C::I2C(int address)
{
    this->address = address;
    Wire.begin(address);
    Wire.onReceive(receiveEvent);
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

int I2C::GetRegister(Node_Registers my_register, void* data)
{
    switch (my_register)
    {
        case Node_Registers::NR_Notification:
        {
            return Notification;
        }
        case Node_Registers::NR_RGB:
        {
            return RGB;
        }
        case Node_Registers::NR_Item:
        {
            strcpy((char*)data, (char*)Item);
            return 0;
        }
        case Node_Registers::NR_Active:
        {
            return Active;
        }
        case Node_Registers::NR_Error:
        {
            strcpy((char*)data, (char*)Error);
            return 0;
        }
        default:
        {
            throw "Invalid Register!"; // This should never happen
        }
    }
}
