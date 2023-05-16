#include <Arduino.h>
#include <string.h>

#include "I2CCommunication.hpp"
#include "MFRC522Reader.hpp"

I2CCommunication* comToHub;
MFRC522Reader* nfcReader;
void setup()
{
    Serial.begin(9600);
    comToHub = new I2CCommunication();
    nfcReader = new MFRC522Reader();
}

void loop()
{
    static char currentCard[49];
    bool status = nfcReader->CheckForCard();
    if (status)
    {
        char card[49];
        nfcReader->ReadCard(card);

        if (strcmp(card, currentCard) != 0)
        {
            strcpy(currentCard, card);
            Serial.println(currentCard);
            comToHub->SendNewItemToHub(card);
        }
    }
    else if (currentCard[0] != '\0')
    {
        Serial.println("Card removed");
        currentCard[0] = '\0';
        comToHub->SendNewItemToHub(currentCard);
    }
}
