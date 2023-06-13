#include "I2C.hpp"
#include <string>
#include <Arduino.h>

#define MAX_I2C_ADDRESSES 126
#define START_I2C_ADDRRESS 9
#define MILLI_TO_SEC 1000
#define I2C_TIMEOUT MILLI_TO_SEC * 100

constexpr int Node_None = 255;
constexpr int Node_Notification = 1; // | SOUND | VIBRATION | LED | RGB | 0 | 0 | 0 | 0 |
constexpr int Node_RGB = 2; // | R | G | B | 0 | 0 | 0 | 0 | 0 |
constexpr int Node_Item = 3; // Points to a char[] of size 48.
constexpr int Node_Active = 4; // 0 = inactive, 1 = active
constexpr int Node_Error = 5;
constexpr int Node_Heartbeat = 6;
constexpr int Node_RequestFrom = 7;
constexpr int Node_Setup = 8;

uint8_t GetAvailableAddress();
void receiveEvent(int howMany);
void RequestEvent();

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
constexpr int Setup_Request = 1;
constexpr int Heartbeat_Request = 2;
uint8_t RequestRegister;
// ===================== //
// ====== REQUEST ====== //
// ===================== //

uint8_t lastRecieved;
bool isAvailable;
volatile int receiveAddress = 0;


typedef struct Address
{
    uint8_t address = 0;
    bool available = false;
    uint32_t lastTime = 0;
} Address;

Address addresses[MAX_I2C_ADDRESSES];

void I2C::InitializeAddresses()
{
    for (int i = 0; i <= START_I2C_ADDRRESS; i++)
    {
        addresses[i].address = i;
        addresses[i].available = false;
        addresses[i].lastTime = 0;
    }
    
    for (int i = START_I2C_ADDRRESS; i < MAX_I2C_ADDRESSES; i++)
    {
        addresses[i].address = i;
        addresses[i].available = true;
        addresses[i].lastTime = 0;
    }
}

I2C::I2C(int address)
{
    this->address = address;
    Wire.begin(address);
    Wire.onReceive(receiveEvent);
    Wire.onRequest(RequestEvent);
}

void I2C::Send(int address,int node_Register, int data)
{
    Wire.end();
    Wire.begin();

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
            strcpy((char*)data, (char*)storedItems[GetLastAddress()].c_str());
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

int I2C::GetLastAddress()
{
    return receiveAddress;
}

uint8_t GetAvailableAddress()
{
    for (int i = 0; i < MAX_I2C_ADDRESSES; i++)
    {
        if (addresses[i].available)
        {
            return addresses[i].address;
        }
    }
    return -1;
}

void I2C::Scan()
{
    for (size_t i = START_I2C_ADDRRESS; i < MAX_I2C_ADDRESSES; i++)
    {
        if(addresses[i].available && addresses[i].lastTime + I2C_TIMEOUT < millis())
        {
            addresses[i].available = false;
            storedItems[i] = "";
        }
    }
}

void receiveEvent(int howMany) // A node sends data, not making a request.
{
    isAvailable = false;

    lastRecieved = Wire.read();
    Serial.println(lastRecieved);
    receiveAddress = Wire.read();

    if (receiveAddress != 127)
    {
        addresses[receiveAddress].lastTime = millis();
        addresses[receiveAddress].available = false;
    }

    char data[MAX_STRING_SIZE];
    size_t i = 0;

    while (Wire.available() && i < MAX_STRING_SIZE)
    {
        data[i] = (char)Wire.read();
        i++;
    }

    data[i] = '\0';

    Serial.println("=== DATA ===");
    Serial.println(lastRecieved);
    Serial.println(receiveAddress);
    Serial.println(data);

    switch (lastRecieved)
    {
        case Node_RequestFrom:
        {
            RequestRegister = (int)data[0];
            break;
        }
        break;

        case Node_Item:
        {
            storedItems[receiveAddress] = std::string(data);
            Serial.println(storedItems[receiveAddress].c_str());
            for (int i = 0; i < MAX_I2C_ADDRESSES; i++)
            {
                Serial.println(addresses[i].address);
                Serial.println(addresses[i].available);
            }
            
            break;
        }
        case Node_Heartbeat:
        {
            Serial.println("Heartbeat");
            break;
        }
        case Node_Error:
        {
            Error = std::string(data);
            break;        
        }
        default:
            break;
    }
    isAvailable = true;
}

void RequestEvent()
{
    switch (RequestRegister)
    {
        case Setup_Request:
        {
            uint8_t retrievedAddress = GetAvailableAddress();
            Wire.write(retrievedAddress);
            RequestRegister = 0;
            break;
        }
        default:
        Wire.write(0);
            break;
    }

}