#include "MFRC522Writer.hpp"
#include "IWriter.hpp"
#include "UARTInterface.hpp"

enum State
{
    PrintToSerial,
    ReadSerial,
    WriteToNFC
};
State currentState = PrintToSerial;
constexpr int RST_PIN = 9;
constexpr int SS_PIN = 10;
constexpr int maxMessageLength = 29;
constexpr int maxConfirmMessageLength = 2;
char message[maxMessageLength + 1];
char confirm[maxConfirmMessageLength + 1];

MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522Writer NFC(mfrc522);
UARTI ui;

void setup()
{
    Serial.begin(9600);
    NFC.Begin();
}

void loop()
{
    switch (currentState)
    {
    case PrintToSerial:
        Serial.println("Please enter your new text , then press Enter...");
        Serial.print("The max text is: ");
        Serial.print(maxMessageLength - 1);
        Serial.println(" Characters");
        currentState = ReadSerial;
        break;

    case ReadSerial:
        if (ui.ReadTextFromSerial(message, maxMessageLength) == 0)
        {
            strcpy(confirm, "");
            Serial.println("Please keep the NFC-TAG on the NFC-Writer. \nEnter _ y _ for confirm or _ n _ for skip, then press Enter... ");
            Serial.println();
            currentState = WriteToNFC;
        }
        break;

    case WriteToNFC:
        if (confirm[0] == 'y')
        {
            NFC.Write(message);
            strcpy(message, "");
            currentState = PrintToSerial;
        }
        else if (confirm[0] == 'n')
        {
            Serial.println("=========== [Cancelled] ===========");
            Serial.println();
            currentState = PrintToSerial;
        }
        else if (confirm[0] != 'y' && confirm[0] != 'n' && strlen(confirm) > 0)
        {
            Serial.println("Wrong input!");
            Serial.println("Please keep the NFC-TAG on the NFC-Writer. \nEnter _ y _ for confirm or _ n _ for skip, then press Enter... ");
            Serial.println();
            strcpy(confirm, "");
        }
        else
        {
            ui.ReadTextFromSerial(confirm, maxConfirmMessageLength);
        }
        break;

    default:
        break;
    }
}
