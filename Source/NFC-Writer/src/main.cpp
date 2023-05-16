#include <Arduino.h>
#include <MFRC522.h>
#include <SPI.h>
#include "MFRC522Writer.h"
#include "IWriter.h"



#define RST_PIN 9 // Configurable, see typical pin layout above
#define SS_PIN 10 // Configurable, see typical pin layout above

 MFRC522 mfrc522(SS_PIN, RST_PIN);
 MFRC522Writer NFC(mfrc522);
 //IWriter NFC(mfrc522);
 //MFRC522Writer *NFC;
 //IWriter NFC;

void setup()
{
    Serial.begin(9600);
    NFC.begin();
}


void loop()
{
    //Serial.println(F("Thissssssssssssssssssssssssssssssssssssssssss"));
    NFC.Write();
}