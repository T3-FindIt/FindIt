#include "MFRC522Reader.hpp"
#include <SPI.h>

constexpr int DEFAULT_KEY = 0xFF;
constexpr int MAX_STRING_ARRAY_LENGTH = 30;
constexpr int LAST_STRING_ARRAY_BYTE = 29;
constexpr int MIFARE_BUFFER_SIZE = 18;
constexpr int TAG_START_ADRESS = 4;
constexpr int RESET_BUFFER_TIME = 50;
constexpr int NFC_TAG_BLOCK_SIZE = 16;
constexpr int DEFAULT_TRAILER_BLOCK = 7;
constexpr int KEY_SIZE = 6;



MFRC522Reader::MFRC522Reader(int SS_PIN, int RST_PIN)
{
    reader = MFRC522(SS_PIN, RST_PIN);
    SPI.begin();
    reader.PCD_Init();
    for (byte i = 0; i < KEY_SIZE; i++)
    {
        key.keyByte[i] = DEFAULT_KEY;
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
        delay(RESET_BUFFER_TIME);
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

// outputstring has to be [30]
int MFRC522Reader::ReadCard(char* outputString, int arrayLength)
{
    if (arrayLength < MAX_STRING_ARRAY_LENGTH)
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

    for (int i = 1; i >= 0; i--)
    {
        byte readBuffer[MIFARE_BUFFER_SIZE];
        byte size = sizeof(readBuffer);

        byte address = TAG_START_ADRESS + i;
        status = (MFRC522::StatusCode)reader.MIFARE_Read(address, readBuffer, &size);
        if (status != MFRC522::STATUS_OK)
        {
            return -1;
        }
        for (int j = 0; j < NFC_TAG_BLOCK_SIZE; j++)
        {
            if (readBuffer[j] == '\n')
            {
                outputString[i * NFC_TAG_BLOCK_SIZE + j] = '\0';
            }
            else
            {
                outputString[i * NFC_TAG_BLOCK_SIZE + j] = readBuffer[j];
            }
        }
        outputString[LAST_STRING_ARRAY_BYTE] = '\0';
    }

    reader.PCD_StopCrypto1();
    return 0;
}