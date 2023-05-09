#include "I2C.hpp"

#include <Arduino.h>
#include <string>


// ===================== //
// ===== REGISTER ===== //
// ===================== //
int Notification = 0x32; // Enable LED 
int RGB;
std::string Item;
int Active;
std::string Error;
// ===================== //
// ===== REGISTER ===== //
// ===================== //

Node_Registers incomingRegister = Node_Registers::NR_None;
Node_Registers lastRecieved;

void receiveEvent(int howMany) // A node sends data, not making a request.
{
    std::string data = "";
    char inbound = ' ';
    int reg = Wire.read();

    while (Wire.available())
    {
        inbound = Wire.read();
        data += inbound;
    }

    switch ((Node_Registers)reg)
    {
        case Node_Registers::NR_Item:
        {
            Item = data;
            break;
        }
        case Node_Registers::NR_Error:
        {
            Error = data;
            break;
        }
        default:
        {
            throw "Invalid Register!"; // This should never happen
        }
    }

    lastRecieved = incomingRegister;
    incomingRegister = Node_Registers::NR_None;
}

I2C::I2C()
{
}

I2C::I2C(int address)
{
    this->address = address;
    
    Wire.begin(address);
    Wire.onReceive(receiveEvent);

    // Serial.begin(9600); !Debug!
}

void I2C::Send(int address,Node_Registers nodeRegister, int data)
{
    Wire.end();
    Wire.begin(); // Start as master

    Wire.beginTransmission(address);
    Wire.write((int)nodeRegister);
    Wire.write(data);
    Wire.endTransmission();

    Wire.end();
    Wire.begin(this->address);
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
            strcpy((char*)data, (char*)Item.c_str());
            return 0;
        }
        case Node_Registers::NR_Active:
        {
            return Active;
        }
        case Node_Registers::NR_Error:
        {
            strcpy((char*)data, (char*)Error.c_str());
            return 0;
        }
        default:
        {
            throw "Invalid Register!"; // This should never happen
        }
    }
}

Node_Registers I2C::GetLastChange()
{
    Node_Registers returnVal = lastRecieved;
    lastRecieved = Node_Registers::NR_None;
    return lastRecieved;
}
