#include "MFRC522Writer.hpp"

constexpr int maxMessageLength = 29;
constexpr int bufferSizeNFCBlock4 = 16;
constexpr int bufferSizeNFCBlock5 = 12;
constexpr byte NFCBlockAddress4 = 4; // NFC block 0-3 are header
constexpr byte NFCBlockAddress5 = 5; // NFC block 0-3 are header
constexpr byte trailerBlock = 7;
constexpr byte keySize = 6;
constexpr int defaultKey = 0xFF;


MFRC522Writer::MFRC522Writer(MFRC522 &nfc) : mfrc522(nfc) {}

MFRC522Writer::~MFRC522Writer()
{
    SPI.end();
}

void MFRC522Writer::Begin()
{
    SPI.begin();
    mfrc522.PCD_Init();

    for (byte i = 0; i < keySize; i++)
    {
        key.keyByte[i] = defaultKey;
    }
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
    if (message == nullptr || !GetCardAvailable())
    {
        return -1;
    }

    char inputString[maxMessageLength] = "";
    char inputString2[maxMessageLength] = "";
    int messageLength = strlen(message);
    if (messageLength > maxMessageLength)
    {
        return -1;
    }
    for (int i = 0; i < messageLength; i++)
    {
        if (i < bufferSizeNFCBlock4)
        {
            inputString[i] = message[i];
        }
        else if (i >= bufferSizeNFCBlock4)
        {
            inputString2[i - bufferSizeNFCBlock4] = message[i];
        }
    }
    int stringLength = strlen(inputString);
    if (stringLength > bufferSizeNFCBlock4)
    {
        return -1;
    }
    int stringLength2 = strlen(inputString2);
    if (stringLength2 > bufferSizeNFCBlock5)
    {
        return -1;
    }
    byte dataBufferNFCBlock4[bufferSizeNFCBlock4] = "";
    byte dataBufferNFCBlock5[bufferSizeNFCBlock5] = "";

    // write data from buffer block Addr 4
    for (int index = 0; index < stringLength; index++)
    {
        dataBufferNFCBlock4[index] = inputString[index];
    }
    if (stringLength < bufferSizeNFCBlock4)
    {
        for (int index = 0; index < bufferSizeNFCBlock4 - stringLength; index++)
        {
            dataBufferNFCBlock4[stringLength + index] = 0;
        }
    }

    // write data from buffer block Addr 5
    for (int index = 0; index < stringLength2; index++)
    {
        dataBufferNFCBlock5[index] = inputString2[index];
    }
    if (stringLength2 < bufferSizeNFCBlock5)
    {
        for (int index = 0; index < bufferSizeNFCBlock5 - stringLength2; index++)
        {
            dataBufferNFCBlock5[stringLength2 + index] = 0;
        }
    }

    if (AuthenticateWrite() == -1 || Transfer(dataBufferNFCBlock4, NFCBlockAddress4, bufferSizeNFCBlock4) == -1 || Transfer(dataBufferNFCBlock5, NFCBlockAddress5, bufferSizeNFCBlock5) == -1)
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

int MFRC522Writer::AuthenticateWrite()
{
    MFRC522::StatusCode status = (MFRC522::StatusCode)mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, trailerBlock, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK)
    {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return -1;
    }
    return 0;
}

int MFRC522Writer::Transfer(byte *inputBlock, byte blockAddr, int bufferSize)
{
    MFRC522::StatusCode status = (MFRC522::StatusCode)mfrc522.MIFARE_Write(blockAddr, inputBlock, bufferSizeNFCBlock4);
    if (status != MFRC522::STATUS_OK)
    {
        Serial.print(F("MIFARE_Write() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return -1;
    }
    Serial.println();
    return 0;
}
