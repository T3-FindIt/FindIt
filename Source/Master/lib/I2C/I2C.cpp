#include "I2C.hpp"
#include <string>
#include <Arduino.h>

#define MAX_I2C_ADDRESSES 127

/// PROTOTYPES
void InitializeAddresses();
int GetAvailableAddress();
bool ReleaseAddress(int address);
///

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

int messageCounter = 0;
int debug_Address = 0;

void receiveEvent(int howMany) // A node sends data, not making a request.
{
    lastRecieved = (Node_Registers)Wire.read();

    debug_Address = Wire.read();

    char data[MAX_STRING_SIZE];

    size_t i = 0;

    while(Wire.available())
    {
        data[i] = Wire.read();
        i++;
    }

    switch (lastRecieved)
    {
    case Node_Registers::NR_Item:
        Item = std::string(data);
        break;
    case Node_Registers::NR_Error:
        Error = std::string(data);
        break;        
    default:
        break;
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

    Serial.begin(9600); //!Debug!
}

void I2C::Send(int address,Node_Registers node_Register, int data)
{
    Wire.end();
    Wire.begin(); // Start as master

    Wire.beginTransmission(address);
    Wire.write((int)node_Register);
    Wire.write(data);
    Wire.endTransmission();

    Wire.end();
    Wire.begin(this->address);
}

void I2C::Request(int address, Node_Registers node_Register, int sizeOfData)
{
    lastRecieved = node_Register;
    Wire.beginTransmission(address);
    Wire.write((int)node_Register);
    Wire.endTransmission();
    Wire.requestFrom(address, sizeOfData);
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

int I2C::debug_GetLastAddress()
{
    return debug_Address;
}

typedef struct Address
{
    int address;
    bool available;
} Address;

Address addresses[MAX_I2C_ADDRESSES];

void InitializeAddresses()
{
    for (int i = 9; i < MAX_I2C_ADDRESSES; i++)
    {
        addresses[i].address = i;
        addresses[i].available = true;
    }
}

int GetAvailableAddress()
{
    for (int i = 9; i < MAX_I2C_ADDRESSES; i++)
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
    for (int i = 9; i < MAX_I2C_ADDRESSES; i++)
    {
        if (addresses[i].address == address)
        {
            addresses[i].available = true;
            return true;
        }
    }
    return false;
}

void I2C::Scan()
{
    Wire.end();
    Wire.begin();
    for (int i = 9; i < MAX_I2C_ADDRESSES; i++)
    {
        Wire.beginTransmission(i);
        Wire.write((int)Node_Registers::NR_RequestForm); Wire.write(0x02);
        Wire.endTransmission();
        Wire.requestFrom(i, 1);
        int heartbeat = Wire.read();
        if(heartbeat == 0)
        {
            ReleaseAddress(i);
        }
    }
    Wire.end();
    Wire.begin(this->address);
    for (int i = 9; i < MAX_I2C_ADDRESSES; i++)
    {
        if(addresses[i].available == false)
        {
            Serial.print("Taken Address: ");
            Serial.println(i);
        }
    }
}
