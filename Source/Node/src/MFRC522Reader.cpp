#include "MFRC522Reader.hpp"
#include <SPI.h>

#define DEFAULT_KEY 0xFF
#define DEFAULT_TRAILER_BLOCK 7


MFRC522Reader::MFRC522Reader(int SS_PIN, int RST_PIN)
{
    reader = MFRC522(SS_PIN, RST_PIN);
    SPI.begin();
    reader.PCD_Init();
    for (byte i = 0; i < 6; i++)
    {
        key.keyByte[i] = 0xFF;
    }
}

MFRC522Reader::~MFRC522Reader()
{
    SPI.end();
}

bool MFRC522Reader::AuthenticateCard()
{
    status = (MFRC522::StatusCode)reader.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, DEFAULT_TRAILER_BLOCK, &key, &(reader.uid));
    if (status != MFRC522::STATUS_OK)
    {
        return false;
    }
    return true;
}

bool MFRC522Reader::CheckForCard()
{
    if (status == MFRC522::STATUS_TIMEOUT)
    {
        reader.PCD_Reset();
        delay(50);
        reader.PCD_Init();
    }
    reader.PICC_IsNewCardPresent();
    if (reader.PICC_ReadCardSerial())
    {
        return true;
    }
    else
    {
        reader.PICC_IsNewCardPresent();
        return reader.PICC_ReadCardSerial();
    }
}

// outputstring has to be [49]
int MFRC522Reader::ReadCard(char* outputString, int arrayLength)
{
    if (arrayLength < 49)
    {
        return -1;
    }
    MFRC522::PICC_Type piccType = reader.PICC_GetType(reader.uid.sak);
    if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI && piccType != MFRC522::PICC_TYPE_MIFARE_1K && piccType != MFRC522::PICC_TYPE_MIFARE_4K)
    {
        return -1;
    }

    if (!AuthenticateCard())
    {
        return -1;
    }

    for (int i = 2; i >= 0; i--)
    {
        byte readBuffer[18];
        byte size = sizeof(readBuffer);

        byte address = 4 + i;
        status = (MFRC522::StatusCode)reader.MIFARE_Read(address, readBuffer, &size);
        if (status != MFRC522::STATUS_OK)
        {
            return -1;
        }
        for (int j = 0; j < 16; j++)
        {
            if (readBuffer[j] == '\n')
            {
                outputString[i * 16 + j] = '\0';
            }
            else
            {
                outputString[i * 16 + j] = readBuffer[j];
            }
        }
        outputString[48] = '\0';
    }

    reader.PCD_StopCrypto1();
    return 0;
}
