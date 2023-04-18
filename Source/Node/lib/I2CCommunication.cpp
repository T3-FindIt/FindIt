#include "I2CCommunication.hpp"

volatile uint8_t activeRegisterAdress = 0;
volatile uint8_t Item[48];
volatile uint8_t RGBValues[3];
volatile uint8_t notificationModeRegistry = 0;
volatile bool notificationState = 0;
volatile uint8_t recievedErrorState = 0;
volatile uint8_t errorState = 0;

void OnRecieve(int HowMany)
{
    switch (activeRegisterAdress)
    {
    case REQUESTFROM_REG:
    {
        //requestfrom shizzle :)
        Wire.flush();
        break;
    }
    case NOTIFICATION_REG:
    {
        notificationModeRegistry = Wire.read();
        break;
    }
    case RGB_REG:
    {
        for (int i = 0; i < 3; i++)
        {
            RGBValues[i] = Wire.read();
        }
        break;
    }
    case ACTIVE_REG:
    {
        notificationState = Wire.read();
        break;
    }
    case ERROR_REG:
    {
        recievedErrorState = Wire.read();
        break;
    }
    default:
    {
        activeRegisterAdress = Wire.read();
        break;
    }
    }
    Wire.flush();
}

void OnRequest()
{
    Wire.beginTransmission(HUB_ADDRESS);
    Wire.write(errorState);
    Wire.endTransmission();
}

I2CCommunication::I2CCommunication()
{
    RGBValues[0] = 0;
    RGBValues[1] = 0;
    RGBValues[2] = 0;
    notificationModeRegistry = 0;
    notificationState = false;
    activeRegisterAdress = 0xFF;
    Wire.begin(NODE_ADRESS);
}

I2CCommunication::~I2CCommunication()
{
    Wire.end();
}

int I2CCommunication::SendNewItemToHub(char* itemString)
{
    Wire.beginTransmission(HUB_ADDRESS);
    Wire.write(ITEM_REG);
    for(int i = 0; i < 48; i++)
    {
        Wire.write(itemString[i]);
    }
    Wire.endTransmission();
}

void I2CCommunication::GetRGBValues(uint8_t* OutputArray)
{
    for(int i = 0 ; i < 3; i++)
    {
        OutputArray[i] = RGBValues[i];
    }
}

uint8_t I2CCommunication::GetNotificationModeRegistry()
{
    return notificationModeRegistry;
}

uint8_t I2CCommunication::GetRecievedErrorState()
{
    return recievedErrorState;
}