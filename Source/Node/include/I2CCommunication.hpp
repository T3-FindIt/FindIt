#ifndef I2CCOMMUNICATION_HPP
#define I2CCOMMUNICATION_HPP

#include <Wire.h>

constexpr int NOTIF_SOUND_POS = 0b0000;
constexpr int NOTIF_VIBRATE_POS = 0b0001;
constexpr int NOTIF_LIGHT_POS = 0b0010;
constexpr int NOTIF_RGB_POS = 0b0100;

constexpr int NOTIFICATION_REG = 0x01;
constexpr int RGB_REG = 0x02;
constexpr int ITEM_REG = 0x03;
constexpr int ACTIVE_REG = 0x04;
constexpr int ERROR_REG = 0x05;
constexpr int HEARTBEAT_REG = 0x06;
constexpr int REQUESTFROM_REG = 0x07;
constexpr int SETUP_REG = 0x08;


class I2CCommunication
{
private:

public:
    I2CCommunication();
    ~I2CCommunication();
    int SendNewItemToHub(char* itemString, size_t stringLength);
    void GetRGBValues(uint8_t* outputArray);
    uint8_t GetNotificationModeRegistry();
    uint8_t GetNotificationState();
    uint8_t GetRecievedErrorState();
    void SendHeartbeat();
};
#endif // I2CCOMMUNICATION_HPP
