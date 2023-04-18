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
#define SETUP_REG 0x06

class I2CCommunication
{
private:
    const int DefaultAdress = 0x7f;
    //uint8_t systemState; //0 is idle, 1 is sending, 2 is recieving //not needed for now
    uint8_t RGBValues[3];
    bool setupState; //0 is idle, 1 is set up for messaging
    uint8_t NotificationRegistryValues;
    uint8_t activeRegister;
    static void OnRecieve(int HowMany);
    static void OnRequest();
public:
    I2CCommunication();
    ~I2CCommunication();
    int SendNewItemToHub(char* itemstring);
    uint8_t GetRGB();
    uint8_t GetNotificationSettings();
};
