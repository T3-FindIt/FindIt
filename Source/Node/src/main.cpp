#include <Arduino.h>
#include "I2CCommunication.hpp"
#include "MFRC522Reader.hpp"

I2CCommunication comToHub;
MFRC522Reader reader;
void setup()
{   
    pinMode(LED_BUILTIN, OUTPUT);
    comToHub = I2CCommunication();
    reader = MFRC522Reader();    
}

void loop()
{
    static std::string currentCard = "";
    if (reader.CheckForCard())
    {
        std::string card = reader.ReadCard();
        if (card != currentCard)
        {
            currentCard = card;
            comToHub->SendNewItemToHub(card);
        }
    }
}
