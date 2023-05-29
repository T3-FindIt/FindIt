#include "MFRC522Writer.h"
#include "IWriter.h"

#define RST_PIN 9 
#define SS_PIN 10 

MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522Writer NFC(mfrc522);

void setup()
{
    Serial.begin(9600);
    NFC.begin();
}

void loop()
{
    NFC.Write();
}