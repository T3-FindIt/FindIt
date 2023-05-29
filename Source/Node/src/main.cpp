#include <Arduino.h>
#include <string.h>

#include "I2CCommunication.hpp"
#include "MFRC522Reader.hpp"

#define CARDSTRINGLENGTH 30
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
    static char currentCard[CARDSTRINGLENGTH] = "12345123451234512345123451234";
    bool status = nfcReader->CheckForCard();
    delay(1000);
    comToHub->SendNewItemToHub(currentCard, CARDSTRINGLENGTH);
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
    // else if (currentCard[0] != '\0')
    // {
    //     Serial.println("Card removed");
    //     currentCard[0] = '\0';
    //     comToHub->SendNewItemToHub(currentCard);
    // }
}
