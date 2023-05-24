#include "I2C.hpp"
#include <string>

#define MAX_I2C_ADDRESSES 127
#define I2C_TIMEOUT 1000
#define MILLIS_TO_SECONDS * 1000

// ===================== //
// ===== REGISTER ===== //
// ===================== //
int Notification = 0x32;
int RGB;
std::string Item;
int Active;
std::string Error;
// ===================== //
// ===== REGISTER ===== //
// ===================== //

Node_Registers lastRecieved = Node_Registers::NR_None;

void receiveEvent(int howMany) // A node sends data, not making a request.
{
    if (howMany == 1)
    {

    }
}

void RequestEvent()
{
    Wire.write(GetAvailableAddress());
}

I2C::I2C(int address)
{
    InitializeAddresses();
    this->address = address;
    Wire.begin(address);
    Wire.onReceive(receiveEvent);
    Wire.onRequest(RequestEvent);
    Wire.setTimeOut(I2C_TIMEOUT MILLIS_TO_SECONDS);

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

void I2C::Scan()
{
    for (int i = 0; i < 127; i++)
    {
        Wire.beginTransmission(i);
        Wire.write((int)Node_Registers::NR_RequestForm);
        Wire.endTransmission();
        int heartbeat = Wire.requestFrom(i, 1);
        if(heartbeat == 0)
        {
            ReleaseAddress(i);
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
    return returnVal;
}

typedef struct Address
{
    int address;
    bool available;
} Address;

Address addresses[MAX_I2C_ADDRESSES];

void InitializeAddresses()
{
    for (int i = 0; i < MAX_I2C_ADDRESSES; i++)
    {
        addresses[i].address = i;
        addresses[i].available = true;
    }
}

int GetAvailableAddress()
{
    for (int i = 0; i < MAX_I2C_ADDRESSES; i++)
    {
        if (addresses[i].available)
        {
            addresses[i].available = false;
            return addresses[i].address;
        }
    }
}

bool ReleaseAddress(int address)
{
    for (int i = 0; i < MAX_I2C_ADDRESSES; i++)
    {
        if (addresses[i].address == address)
        {
            addresses[i].available = true;
            return true;
        }
    }
    return false;
}