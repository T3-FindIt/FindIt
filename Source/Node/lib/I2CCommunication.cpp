#include "I2CCommunication.hpp"

void I2CCommunication::OnRecieve(int HowMany)
{
    switch (activeRegister)
    {
    case REQUESTFROM_REG:
        break;
    case NOTIFICATION_REG:
        break;
    case RGB_REG:
        break;
    case ITEM_REG:
        break;
    case ERROR_REG:
        break;
    case SETUP_REG:
        break;
    
    default:
        break;
    }
}

void I2CCommunication::OnRequest()
{

}

I2CCommunication::I2CCommunication()
{
    RGBValues[0] = 0;
    RGBValues[1] = 0;
    RGBValues[2] = 0;
    setupState = false;
    NotificationRegistryValues = 0;
    activeRegister = SETUP_REG;
    Wire.begin(DefaultAdress);
    Wire.onReceive(OnRecieve);
    Wire.onRequest(OnRequest);
    int newAdress = 0;
    Wire.requestFrom(0x08, 1); 
    while (!Wire.available())
    {
        //do nothing
    }
    newAdress = Wire.read();
    Wire.end();
    Wire.begin(newAdress);
    Wire.onReceive(OnRecieve);
    Wire.onRequest(OnRequest);
}

I2CCommunication::~I2CCommunication()
{

}

uint8_t I2CCommunication::GetNotificationSettings()
{

}

uint8_t I2CCommunication::GetRGB()
{

}

int I2CCommunication::SendNewItemToHub(char* newItem)
{

}