#include "I2CCommunication.hpp"
#include <Arduino.h>

constexpr int HUB_ADDRESS = 0x08;
constexpr int DEFAULT_NODE_ADRESS = 0x7F;

constexpr int REQUESTFROM_SETUP_REG = 0x01;
constexpr int REQUESTFROM_HEARTBEAT_REG = 0x02;

constexpr int MIN_ADRESS = 0x09;
constexpr int MAX_ADRESS = 0x7E;
constexpr int ADRESS_NOT_SET = 0x00;
constexpr int WAIT_FOR_SETUP_DELAY = 500;
constexpr int MAX_STRING_LENGTH = 30;
constexpr int RGB_SIZE = 3;

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
        // Serial.print("Request from");
        // Serial.println(requestfromRegister);
        break;
    }
    case NOTIFICATION_REG:
    {
        // Serial.println("Notification");
        notificationModeRegistry = Wire.read();
        break;
    }
    case RGB_REG:
    {
        // Serial.println("RGB");
        for (int i = 0; i < RGB_SIZE; i++)
        {
            RGBValues[i] = Wire.read();
        }
        break;
    }
    case ACTIVE_REG:
    {
        // Serial.println("Active");
        notificationState = Wire.read();
        Serial.println(notificationState);
        break;
    }
    case ITEM_REG:
    {
        // Serial.println("Item");
        //DO NOTHING
        break;
    }
    case ERROR_REG:
    {
        // Serial.println("Error");
        recievedErrorState = Wire.read();
        break;
    }
    default:
    {
        // Serial.println("Default");
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
    // Serial.begin(9600); //DEBUG
    activeRegisterAdress = 0;
    RGBValues[0] = 0;
    RGBValues[1] = 0;
    RGBValues[2] = 0;
    notificationModeRegistry = 0;
    notificationState = false;
    recievedErrorState = 0;
    errorState = 0;
    Wire.begin(DEFAULT_NODE_ADRESS);
    Wire.requestFrom(HUB_ADDRESS, 1);
    while (Wire.available() == 0)
    {
        //DO NOTHING
    }
    Wire.read();
    while (activeAdress == ADRESS_NOT_SET || activeAdress < MIN_ADRESS || activeAdress > MAX_ADRESS)
    {
        Wire.beginTransmission(HUB_ADDRESS);
        Wire.write(REQUESTFROM_REG);
        Wire.write(DEFAULT_NODE_ADRESS);
        Wire.write(REQUESTFROM_SETUP_REG);
        Wire.endTransmission();
        Wire.requestFrom(HUB_ADDRESS, 1);
        if (Wire.available() == 0)
        {
            // Serial.println("Waiting for adress");
            delay(WAIT_FOR_SETUP_DELAY);
            //DO NOTHING
        }
        else
        {
            activeAdress = Wire.read();
        }
    }
    Wire.begin(activeAdress);
    Wire.onReceive(RecieveEvent);
    Wire.onRequest(RequestEvent);
    // Serial.println(activeAdress);
    SendHeartbeat();

}

I2CCommunication::~I2CCommunication()
{
    Wire.end();
}

int I2CCommunication::SendNewItemToHub(char* itemString, size_t stringLength)
{
    if (activeAdress == 0)
    {
        return -1;
    }
    if (stringLength > MAX_STRING_LENGTH)
    {
        return -2;
    }
    Wire.beginTransmission(HUB_ADDRESS);
    Wire.write(ITEM_REG);
    Wire.write(activeAdress);
    for (int i = 0; i < MAX_STRING_LENGTH; i++)
    {
        Wire.write(itemString[i]);
        if (itemString[i + 1] == '\0')
        {
            i = MAX_STRING_LENGTH;
        }
    }
    Wire.endTransmission();
    return 0;
}

void I2CCommunication::GetRGBValues(uint8_t* outputArray)
{
    for (int i = 0 ; i < RGB_SIZE; i++)
    {
        outputArray[i] = RGBValues[i];
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

void I2CCommunication::SendHeartbeat()
{
    Wire.beginTransmission(HUB_ADDRESS);
    Wire.write(HEARTBEAT_REG);
    Wire.write(activeAdress);
    Wire.write(0);
    Wire.endTransmission();
}