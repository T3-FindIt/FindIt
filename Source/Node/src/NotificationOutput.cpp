#include "NotificationOutput.hpp"

constexpr uint8_t RGB_SIZE = 3; // PINS CAN STILL BE CHANGED (for MVP these pins are fine)
constexpr uint8_t LED_PIN = 7;
constexpr uint8_t RGB_PIN[3] = { A1, A2, A3 };
constexpr uint8_t BUZZER_PIN = 6;
constexpr uint8_t VIBRATION_PIN = 3;

constexpr uint8_t buzzerSettingMask = 0b00000001;
constexpr uint8_t vibrationSettingMask = 0b00000010;
constexpr uint8_t ledSettingMask = 0b00000100;
constexpr uint8_t rgbSettingMask = 0b00001000;

NotificationOutput::NotificationOutput(uint8_t Settings)
{
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    for (int i = 0; i < RGB_SIZE; i++)
    {
        pinMode(RGB_PIN[i], OUTPUT);
    }
    pinMode(VIBRATION_PIN, OUTPUT);
    this->UpdateSettings(Settings);
}

NotificationOutput::~NotificationOutput()
{
}

int NotificationOutput::On()
{
    if (vibrationSettingOn)
    {
        digitalWrite(VIBRATION_PIN, HIGH);
    }
    if (buzzerSettingOn)
    {
        Serial.println("Buzzer");
        analogWrite(BUZZER_PIN, UINT8_MAX);
    }
    if (ledSettingOn)
    {
        digitalWrite(LED_PIN, HIGH);
    }
    if (rgbSettingOn)
    {
        for (int i = 0; i < RGB_SIZE; i++)
        {
            analogWrite(RGB_PIN[i], RgbValues[i]);
        }
    }
    return 0;
}

int NotificationOutput::Off()
{
    digitalWrite(VIBRATION_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
    for (int i = 0; i < RGB_SIZE; i++)
    {
        analogWrite(RGB_PIN[i], 0);
    }
    return 0;
}

int NotificationOutput::UpdateSettings(uint8_t Settings)
{
    ledSettingOn = (Settings & ledSettingMask) >> 2;
    rgbSettingOn = (Settings & rgbSettingMask) >> 3;
    buzzerSettingOn = (Settings & buzzerSettingMask);
    vibrationSettingOn = (Settings & vibrationSettingMask) >> 1;
    return 0;
}

int NotificationOutput::SetRgbValues(uint8_t RgbValues[3])
{
    for (int i = 0; i < RGB_SIZE; i++)
    {
        this->RgbValues[i] = RgbValues[i];
    }
    return 0;
}
