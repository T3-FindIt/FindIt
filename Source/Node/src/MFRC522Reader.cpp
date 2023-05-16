#include "MFRC522Reader.hpp"

#define DEFAULT_SS_PIN 10
#define DEFAULT_RST_PIN 9
#define DEFAULT_KEY 0xFF
#define DEFAULT_TRAILER_BLOCK 7

void initReader(int SS_PIN, int RST_PIN)
{
    reader = MFRC522(SS_PIN, RST_PIN);
    reader.PCD_Init();
    for (byte i = 0; i < 6; i++)
    {
        key.keyByte[i] = 0xFF;
    }
}

bool AuthenticateCard(MFRC522::StatusCode status)
{
    status = (MFRC522::StatusCode)mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, DEFAULT_TRAILER_BLOCK, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK)
    {
        return false;
    }
    return true;
}

MFRC522Reader::MFRC522Reader()
{
    initReader(DEFAULT_SS_PIN, DEFAULT_RST_PIN);
}

MFRC522Reader::MFRC522Reader(int SS_PIN, int RST_PIN)
{
    initReader(SS_PIN, RST_PIN);
}

MFRC522Reader::~MFRC522Reader()
{
    delete reader;
    delete key;
}

bool MFRC522Reader::CheckForCard()
{
    if (!reader.PICC_IsNewCardPresent())
    {
        return false;
    }
    if (!reader.PICC_ReadCardSerial())
    {
        return false;
    }
    return true;
}

std::string MFRC522Reader::ReadCard()
{
    std::string OutputString[49];
    MFRC522::StatusCode status;

    MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI && piccType != MFRC522::PICC_TYPE_MIFARE_1K && piccType != MFRC522::PICC_TYPE_MIFARE_4K)
    {
        return "";
    }

    if (!AuthenticateCard(status))
    {
        return "";
    }

    for (int i = 2; i >= 0; i--)
    {
        {
            byte readBuffer[18];
            byte size = sizeof(readBuffer);

            byte address = 4 + i;
            status = (MFRC522::StatusCode)mfrc522.MIFARE_Read(address, readBuffer, &size);
            if (status != MFRC522::STATUS_OK)
            {
                Serial.print(F("MIFARE_Read() failed: "));
                Serial.println(mfrc522.GetStatusCodeName(status));
            }
            for (int j = 0; j < 16; j++)
            {
                if (readBuffer[j] == '\n')
                {
                    OutputString[i * 16 + j] = '\0';
                }
                else
                {
                    OutputString[i * 16 + j] = readBuffer[j];
                }
            }
        }
    }
    return OutputString;
}
