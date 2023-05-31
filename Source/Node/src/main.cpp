#include <Arduino.h>
#include <string.h>

#include "I2CCommunication.hpp"
#include "MFRC522Reader.hpp"

#define CARDSTRINGLENGTH 49 //constexpr
I2CCommunication* comToHub; //Raw pointers are not necessary. Use constants or smart pointers
MFRC522Reader* nfcReader;
void setup()
{
    Serial.begin(9600);
    comToHub = new I2CCommunication();
    nfcReader = new MFRC522Reader();
}

void loop()
{
    static char currentCard[CARDSTRINGLENGTH];
    bool status = nfcReader->CheckForCard();
    if (status)
    {
        char card[49]; //C string? Why not std::string?
        nfcReader->ReadCard(card, CARDSTRINGLENGTH); //C style function. This should return an std::string or an std::optional with an std::string

        if (strcmp(card, currentCard) != 0) //This logic is state behaviour and should not happen in main.
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
