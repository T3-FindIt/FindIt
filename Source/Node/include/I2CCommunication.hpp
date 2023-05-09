#ifndef I2CCOMMUNICATION_HPP
#define I2CCOMMUNICATON_HPP

#include <Wire.h>

#define NOTIF_SOUND_POS 0b0000
#define NOTIF_VIBRATE_POS 0b0001
#define NOTIF_LIGHT_POS 0b0010
#define NOTIF_RGB_POS 0b0100

#define REQUESTFROM_REG 0x00
#define NOTIFICATION_REG 0x01
#define RGB_REG 0x02
#define ITEM_REG 0x03
#define ACTIVE_REG 0x04
#define ERROR_REG 0x05

#define HUB_ADDRESS 0x08
#define NODE_ADRESS 0x7F

class I2CCommunication
{
private:

public:
    I2CCommunication();
    ~I2CCommunication();
    int SendNewItemToHub(char* itemstring);
    void GetRGBValues(uint8_t* OutputArray);
    uint8_t GetNotificationModeRegistry();
    uint8_t GetNotificationState();
    uint8_t GetRecievedErrorState();
};
#endif