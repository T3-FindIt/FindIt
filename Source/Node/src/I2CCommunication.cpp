#include "I2CCommunication.hpp"
#include <Arduino.h>
volatile uint8_t activeRegisterAdress;
volatile uint8_t RGBValues[3];
volatile uint8_t notificationModeRegistry;
volatile bool notificationState;
volatile uint8_t recievedErrorState;
volatile uint8_t errorState;

void OnRecieve(int HowMany)
{
    Serial.println("HI");
    while (Wire.available() > 0)
    {
        Wire.read();
    } //TEST RECIEVED MSG
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
    Serial.begin(9600);
    activeRegisterAdress = 0xFF;
    RGBValues[0] = 0;
    RGBValues[1] = 0;
    RGBValues[2] = 0;
    notificationModeRegistry = 0;
    notificationState = false;
    recievedErrorState = 0;
    errorState = 0;
    Wire.begin(NODE_ADRESS);
    Wire.onReceive(OnRecieve);
    Wire.onRequest(OnRequest);
}

I2CCommunication::~I2CCommunication()
{
    Wire.end();
}

int I2CCommunication::SendNewItemToHub(char* itemString)
{
    Wire.beginTransmission(HUB_ADDRESS);
    Wire.write(ITEM_REG);
    bool endofstring = false;
    for (int i = 0; i < 49; i++)
    {
        if (!endofstring)
        {
            Wire.write(itemString[i]);
        }
        if (itemString[i+1] == '\0')
        {
            endofstring = true;
        }
    }
    Wire.endTransmission();

    return 0;
}

void I2CCommunication::GetRGBValues(uint8_t* OutputArray)
{
    for (int i = 0 ; i < 3; i++)
    {
        OutputArray[i] = RGBValues[i];
    }
}

uint8_t I2CCommunication::GetNotificationModeRegistry()
{
    return notificationModeRegistry;
}

uint8_t I2CCommunication::GetNotificationState()
{
    return notificationState;
}


uint8_t I2CCommunication::GetRecievedErrorState()
{
    return recievedErrorState;
}
