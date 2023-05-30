#include "MFRC522Writer.hpp"
#include "IWriter.hpp"
#include "UI.hpp"

#define RST_PIN 9
#define SS_PIN 10

MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522Writer NFC(mfrc522);
UI ui;
char message[30];

void setup()
{
    Serial.begin(9600);
    NFC.Begin();
}

void loop()
{
    Serial.println("Please enter your new text , then press Enter...");
    if (ui.ReadTextFromSerial(message, 29) == 0)
    {
        NFC.Write(message);
    }
}
