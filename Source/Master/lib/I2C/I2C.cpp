#include "I2C.hpp"
#include <string>
#include <Arduino.h>

#define MAX_I2C_ADDRESSES 118
#define START_I2C_ADDRRESS 0

/// Pain
constexpr int Node_None = 255;
constexpr int Node_RequestForm = 0;
constexpr int Node_Notification = 1; // | SOUND | VIBRATION | LED | RGB | 0 | 0 | 0 | 0 |
constexpr int Node_RGB = 2; // | R | G | B | 0 | 0 | 0 | 0 | 0 |
constexpr int Node_Item = 3; // Points to a char[] of size 48.
constexpr int Node_Active = 4; // 0 = inactive, 1 = active
constexpr int Node_Error = 5;

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
std::string storedItems[MAX_I2C_ADDRESSES];
int Active;
std::string Error;
// ===================== //
// ===== REGISTER ===== //
// ===================== //

// ===================== //
// ====== REQUEST ====== //
// ===================== //
#define REQUEST_HEARTBEAT 0x02
// ===================== //
// ====== REQUEST ====== //
// ===================== //

uint8_t lastRecieved;
bool isAvailable;

volatile int receiveAddress = 0;

void receiveEvent(int howMany) // A node sends data, not making a request.
{
    isAvailable = false;

    Wire.print("Available Bytes:");
    Serial.println(Wire.available());

    lastRecieved = Wire.read();
    receiveAddress = Wire.read();
    
    Serial.print("Recieved Address");
    Serial.println(receiveAddress);

    char data[MAX_STRING_SIZE];

    size_t i = 0;

    while(Wire.available() && i < MAX_STRING_SIZE)
    {
        data[i] = (char)Wire.read();
        i++;
    }

    data[i] = '\0';

    switch (lastRecieved)
    {
    case Node_Item:
        storedItems[receiveAddress] = std::string(data);
        Serial.println(storedItems[receiveAddress].c_str());
        break;
    case Node_Error:
        Error = std::string(data);
        break;        
    default:
        break;
    }
    isAvailable = true;
}

void RequestEvent()
{
    int retrievedAddress = GetAvailableAddress();
    Serial.print("Retrieved Address");
    Serial.println(retrievedAddress);
    Wire.write(retrievedAddress);
}

I2C::I2C(int address)
{
    InitializeAddresses();
    this->address = address;
    Wire.begin(address);
    Wire.onReceive(receiveEvent);
    Wire.onRequest(RequestEvent);

    Serial.begin(9600);
}

void I2C::Send(int address,int node_Register, int data)
{
    Wire.end();
    Wire.begin(); // Start as master

    Wire.beginTransmission(address);
    Wire.write(node_Register);
    Wire.write(data);
    Wire.endTransmission();

    Wire.end();
    Wire.begin(this->address);
}

void I2C::Request(int address, int node_Register, int sizeOfData)
{
    lastRecieved = node_Register;
    Wire.beginTransmission(address);
    Wire.write(node_Register);
    Wire.endTransmission();
    Wire.requestFrom(address, sizeOfData);
}

int I2C::GetRegister(int my_register, void* data)
{
    switch (my_register)
    {
        case Node_Notification:
        {
            return Notification;
        }
        case Node_RGB:
        {
            return RGB;
        }
        case Node_Item:
        {
            strcpy((char*)data, (char*)storedItems[debug_GetLastAddress()].c_str());
            return 0;
        }
        case Node_Active:
        {
            return Active;
        }
        case Node_Error:
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

int I2C::GetLastChange()
{
    int returnVal = lastRecieved;
    lastRecieved = Node_None;
    return returnVal;
}

bool I2C::IsAvailable()
{
    return isAvailable;
}

int I2C::debug_GetLastAddress()
{
    return receiveAddress;
}

typedef struct Address
{
    uint8_t address;
    bool available;
} Address;

Address volatile addresses[MAX_I2C_ADDRESSES];

void InitializeAddresses()
{

    for (int i = START_I2C_ADDRRESS; i < MAX_I2C_ADDRESSES; i++)
    {
        addresses[i].address = i + 9; // Offset
        addresses[i].available = true;
        storedItems[i] = "";
    }
}

int GetAvailableAddress()
{
    for (int i = START_I2C_ADDRRESS; i < MAX_I2C_ADDRESSES; i++)
    {
        if (addresses[i].available)
        {
            addresses[i].available = false;
            Serial.print("Claimed Address: ");
            Serial.println(addresses[i].address);
            return addresses[i].address;
        }
    }
    return -1;
}

bool ReleaseAddress(int address)
{
    for (int i = START_I2C_ADDRRESS; i < MAX_I2C_ADDRESSES; i++)
    {
        if (addresses[i].address == address)
        {
            addresses[i].available = true;
            storedItems[i] = "";
            return true;
        }
    }
    return false;
}

void I2C::Scan()
{
    Wire.end();
    Wire.begin();
    for (int i = START_I2C_ADDRRESS; i < MAX_I2C_ADDRESSES; i++)
    {
        if(addresses[i].available == false)
        {
            Wire.beginTransmission(i);
            Wire.write(Node_RequestForm);
            Wire.write(REQUEST_HEARTBEAT);
            Wire.endTransmission();
            
            Wire.requestFrom(i, 1);
            int heartbeat = Wire.read();
            if(heartbeat == -1)
            {
                ReleaseAddress(i);
            }
            else
            {
                // Serial.print("Heartbeat: ");
                // Serial.println(heartbeat);
            }
        }
    }
    Wire.end();
    Wire.begin(this->address);
    for (int i = START_I2C_ADDRRESS; i < MAX_I2C_ADDRESSES; i++)
    {
        if(addresses[i].available == false)
        {
            // Serial.print("Taken Address: ");
            // Serial.println(i);
        }
    }
}
