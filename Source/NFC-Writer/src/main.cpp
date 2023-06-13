#include "MFRC522Writer.hpp"
#include "IWriter.hpp"
#include "UATRInterface.hpp"

enum State
{
    PrintToSerial,
    ReadSerial,
    WriteToNFC
};
State currentState = PrintToSerial;
constexpr int RST_PIN = 9;
constexpr int SS_PIN = 10;
constexpr int max_message_length = 29;
constexpr int max_confirm_message_length = 2;
char message[30];
char confirm[3];

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
        Serial.print(max_message_length - 1);
        Serial.println(" Characters");
        currentState = ReadSerial;
        break;

    case ReadSerial:
        if (ui.ReadTextFromSerial(message, max_message_length) == 0)
        {
            confirm[0] = '\0';
            Serial.println("Please keep the NFC-TAG on the NFC-Writer. \nEnter _ y _ for confirm or _ n _ for skip, then press Enter... ");
            Serial.println();
            currentState = WriteToNFC;
        }
        break;

    case WriteToNFC:
        if (confirm[0] == 'y')
        {
            NFC.Write(message);
            currentState = PrintToSerial;
        }
        else if (confirm[0] == 'n')
        {
            Serial.println("=========== [Cancelled] ===========");
            Serial.println();
            currentState = PrintToSerial;
        }
        else if(confirm[0] != 'y' && confirm[0] != 'n' && strlen(confirm) > 0)
        {
            Serial.println("Wrong input!");
            Serial.println("Please keep the NFC-TAG on the NFC-Writer. \nEnter _ y _ for confirm or _ n _ for skip, then press Enter... ");
            Serial.println();
            confirm[0] = '\0';
        }
        else
        {
            ui.ReadTextFromSerial(confirm, max_confirm_message_length);
        }
        break;

    default:
        break;
    }
}
