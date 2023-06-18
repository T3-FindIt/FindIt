#include <Arduino.h>
#include <string.h>

#include "I2CCommunication.hpp"
#include "MFRC522Reader.hpp"
#include "NotificationOutput.hpp"

constexpr int CARDSTRINGLENGTH = 30;

constexpr uint32_t heartbeatTime = 99000;
I2CCommunication* comToHub;
MFRC522Reader* nfcReader;
NotificationOutput* notificationOutput;

char currentCard[CARDSTRINGLENGTH] = "\0";

void setup()
{
    Serial.begin(9600);
    comToHub = new I2CCommunication();
    nfcReader = new MFRC522Reader();
    notificationOutput = new NotificationOutput(0);
}

void loop()
{
    static uint32_t lastSendTime = 0;


    uint8_t currentSettings = 0;
    uint8_t currentRGBValues[3] = {0, 0, 0};

    // uint8_t currentSettings = 0b00001111; //testing
    // uint8_t currentRGBValues[3] = {255, 255, 255};
    // notificationOutput->UpdateSettings(currentSettings);
    // notificationOutput->SetRgbValues(currentRGBValues);
    // notificationOutput->On();
    // delay(1000);
    // notificationOutput->Off();
    // delay(1000);

    if (comToHub->GetNotificationModeRegistry() != currentSettings) // NEEDS hub implementation
    {
        currentSettings = comToHub->GetNotificationModeRegistry();
        notificationOutput->UpdateSettings(currentSettings);
    }
    comToHub->GetRGBValues(currentRGBValues);
    notificationOutput->SetRgbValues(currentRGBValues);

    if (comToHub->GetNotificationState() == 0)
    {
        notificationOutput->Off();
    }
    else
    {
        notificationOutput->On();
    }


    bool status = nfcReader->CheckForCard();
    if (status)
    {

        char card[CARDSTRINGLENGTH];
        nfcReader->ReadCard(card, CARDSTRINGLENGTH);

        if (strcmp(card, currentCard) != 0)
        {
            strcpy(currentCard, card);
            Serial.println(currentCard);
            comToHub->SendNewItemToHub(card, CARDSTRINGLENGTH);
        }
    }
    else if (currentCard[0] != '\0')
    {
        Serial.println("Card removed");
        currentCard[0] = '\0';
        comToHub->SendNewItemToHub(currentCard, CARDSTRINGLENGTH);
    }

    if (millis() - lastSendTime > heartbeatTime)
    {
        lastSendTime = millis();
        comToHub->SendHeartbeat();
    }


}
