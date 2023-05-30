#include "MFRC522Writer.hpp"

MFRC522Writer::MFRC522Writer(MFRC522 &nfc)
{
    mfrc522 = nfc;
}

MFRC522Writer::~MFRC522Writer()
{
    SPI.end();
}

void MFRC522Writer::Begin()
{
    while (!Serial)
    {
    }
    SPI.begin();
    mfrc522.PCD_Init();

    for (byte i = 0; i < 6; i++)
    {
        key.keyByte[i] = 0xFF;
    }
    DumpByteArray(key.keyByte, MFRC522::MF_KEY_SIZE);
}

bool MFRC522Writer::GetCardAvailable()
{
    mfrc522.PICC_IsNewCardPresent();
    if (!mfrc522.PICC_ReadCardSerial())
    {
        mfrc522.PICC_IsNewCardPresent();
        return mfrc522.PICC_ReadCardSerial();
    }
    return true;
}

int MFRC522Writer::Write(char *message)
{
    if (!GetCardAvailable())
    {
        return -1;
    }
    if (message == nullptr)
    {
        return -1;
    }

    byte blockAddr4 = 4;
    byte blockAddr5 = 5;
    char inputString[maxMessageLength] = "";
    char inputString2[maxMessageLength] = "";
    int messageLength = strlen(message);
    for (int i = 0; i < messageLength; i++)
    {
        if (i < bufferSize)
        {
            inputString[i] = message[i];
        }
        else if (i >= bufferSize)
        {
            inputString2[i - bufferSize] = message[i];
        }
    }
    int stringLength = strlen(inputString);
    int stringLength2 = strlen(inputString2);
    byte dataBuffer[bufferSize] = "";
    byte dataBuffer2[bufferSize2] = "";

    // write data from buffer block Addr 4
    for (int j = 0; j < stringLength; j++)
    {
        dataBuffer[j] = inputString[j];
    }
    if (stringLength < bufferSize)
    {
        for (int j = 0; j < bufferSize - stringLength; j++)
        {
            dataBuffer[stringLength + j] = 0;
        }
    }

    // write data from buffer block Addr 5
    for (int j = 0; j < stringLength2; j++)
    {
        dataBuffer2[j] = inputString2[j];
    }
    if (stringLength2 < bufferSize2)
    {
        for (int j = 0; j < bufferSize2 - stringLength2; j++)
        {
            dataBuffer2[stringLength2 + j] = 0;
        }
    }

    if (AuthForWrite() == -1)
    {
        return -1;
    }
    if (Transfer(dataBuffer, blockAddr4) == -1)
    {
        return -1;
    }
    if (Transfer(dataBuffer2, blockAddr5) == -1)
    {
        return -1;
    }
    Serial.println(F("=========== [SUCCESS] ==========="));
    Serial.print("The New Data Is: ");
    Serial.println(message);
    Serial.println();
    mfrc522.PCD_StopCrypto1();
    return 0;
}

int MFRC522Writer::DumpByteArray(byte *buffer, byte bufferSize)
{
    if (buffer == nullptr || bufferSize == 0)
    {
        return -1;
    }
    for (byte i = 0; i < bufferSize; i++)
    {
        buffer[i] = 0xFF;
    }
    return 0;
}

int MFRC522Writer::AuthForWrite()
{
    byte trailerBlock = 7;
    // Serial.println(F("Authenticating again using key B..."));
    MFRC522::StatusCode status = (MFRC522::StatusCode)mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, trailerBlock, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK)
    {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return -1;
    }
    return 0;
}

int MFRC522Writer::Transfer(byte *inputBlock, byte blockAddr)
{
    MFRC522::StatusCode status = (MFRC522::StatusCode)mfrc522.MIFARE_Write(blockAddr, inputBlock, bufferSize);
    if (status != MFRC522::STATUS_OK)
    {
        Serial.print(F("MIFARE_Write() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return -1;
    }
    Serial.println();
    return 0;
}
