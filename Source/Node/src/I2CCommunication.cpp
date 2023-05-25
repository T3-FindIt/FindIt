#include "I2CCommunication.hpp"
#include <Arduino.h>

#define HUB_ADDRESS 0x08
#define DEFAULT_NODE_ADRESS 0x7F

#define REQUESTFROM_SETUP_REG 0x01
#define REQUESTFROM_HEARTBEAT_REG 0x02

volatile uint8_t activeRegisterAdress;
volatile uint8_t RGBValues[3];
volatile uint8_t notificationModeRegistry;
volatile bool notificationState;
volatile uint8_t recievedErrorState;
volatile uint8_t errorState;
volatile uint8_t activeAdress;
volatile uint8_t requestfromRegister;



void RecieveEvent(int HowMany)
{
    activeRegisterAdress = Wire.read();
    switch (activeRegisterAdress)
    {
    case REQUESTFROM_REG:
    {
        requestfromRegister = Wire.read();
        Serial.println("Request from");
        break;
    }
    case NOTIFICATION_REG:
    {
        Serial.println("Notification");
        notificationModeRegistry = Wire.read();
        break;
    }
    case RGB_REG:
    {
        Serial.println("RGB");
        for (int i = 0; i < 3; i++)
        {
            RGBValues[i] = Wire.read();
        }
        break;
    }
    case ACTIVE_REG:
    {
        Serial.println("Active");
        notificationState = Wire.read();
        break;
    }
    case ITEM_REG:
    {
        Serial.println("Item");
        //DO NOTHING
        break;
    }
    case ERROR_REG:
    {
        Serial.println("Error");
        recievedErrorState = Wire.read();
        break;
    }
    default:
    {
        Serial.println("Default");
        //DO NOTHING
        break;
    }
    }
    while (Wire.available() > 0)
    {
        Wire.read();
    }   
}

void RequestEvent()
{
    if (requestfromRegister == REQUESTFROM_HEARTBEAT_REG)
    {
        Wire.write(activeAdress);
    }
    else
    {
        Wire.write(errorState);
    }
}

I2CCommunication::I2CCommunication()
{
    Serial.begin(9600);
    activeRegisterAdress = 0;
    RGBValues[0] = 0;
    RGBValues[1] = 0;
    RGBValues[2] = 0;
    notificationModeRegistry = 0;
    notificationState = false;
    recievedErrorState = 0;
    errorState = 0;
    Wire.begin(DEFAULT_NODE_ADRESS);
    while(activeAdress == 0|| activeAdress < 0x09 || activeAdress > 0x6F)
    {
        Wire.beginTransmission(HUB_ADDRESS);
        Wire.write(REQUESTFROM_REG);
        Wire.write(REQUESTFROM_SETUP_REG);
        Wire.endTransmission();
        Wire.requestFrom(HUB_ADDRESS, 1);
        if (Wire.available() == 0)
        {
            Serial.println("Waiting for adress");
            delay(500);
            //DO NOTHING
        }
        activeAdress = Wire.read();
    }
    Wire.begin(activeAdress);
    Wire.onReceive(RecieveEvent);
    Wire.onRequest(RequestEvent);
    Serial.println(activeAdress);
}

I2CCommunication::~I2CCommunication()
{
    Wire.end();
}

int I2CCommunication::SendNewItemToHub(char* itemString)
{
    if (activeAdress == 0)
    {
        return -1;
    }
    Wire.beginTransmission(HUB_ADDRESS);
    Wire.write(ITEM_REG);
    Wire.write(activeAdress);
    bool endofstring = false;
    for (int i = 0; i < 49; i++)
    {
        if (!endofstring)
        {
            Wire.write(itemString[i]);
        }
        if (itemString[i + 1] == '\0')
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
