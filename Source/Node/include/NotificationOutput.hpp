#ifndef NOTIFICATIONOUTPUT_HPP
#define NOTIFICATIONOUTPUT_HPP

#include "IOutput.hpp"
#include <Arduino.h>

class NotificationOutput : public IOutput
{
private:
    int RgbValues[3];
    uint8_t settings;
    bool ledSettingOn;
    bool rgbSettingOn;
    bool buzzerSettingOn;
    bool vibrationSettingOn;
public:
    NotificationOutput(uint8_t Settings);
    ~NotificationOutput();
    int On();
    int Off();
    int UpdateSettings(uint8_t Settings);
    int SetRgbValues(uint8_t RgbValues[3]);
};
#endif // NOTIFICATIONOUTPUT_HPP
