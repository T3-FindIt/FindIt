#include "MFRC522Writer.hpp"

MFRC522Writer::MFRC522Writer(MFRC522& nfc) :
mfrc522(nfc)
{
    //mfrc522 = nfc; //Writing a copy of a reference unintentionally. I provided a fix (probably, haven't compiled it)
}

MFRC522Writer::~MFRC522Writer()
{
    SPI.end();
}

void MFRC522Writer::Begin() //What happens if I never call this function? Should we be concious of that?
{
    while (!Serial) //Infinite loop? Why?
    {
    }

    SPI.begin();
    mfrc522.PCD_Init();

    for (byte i = 0; i < 6; i++)
    {
        key.keyByte[i] = 0xFF;
    }
    DumpByteArray(key.keyByte, MFRC522::MF_KEY_SIZE); //The above for loop already does this.
    //Furthermore, a function called DumpByteArray should not write 0xFF into every single one of it's bytes.
    //It should dump ByteArray a into ByteArray B. Which could also just be done with a memcpy, or with an std::copy
}

bool MFRC522Writer::GetCardAvailable()
{
    mfrc522.PICC_IsNewCardPresent(); //Should probably add a comment here on why you do this. I know, but not everyone will
    if (!mfrc522.PICC_ReadCardSerial())
    {
        mfrc522.PICC_IsNewCardPresent();
        return mfrc522.PICC_ReadCardSerial();
    }
    return true;
}

int MFRC522Writer::Write(char *message) //C string??? Why?
{
    if (!GetCardAvailable()) //Kudos: Good sanity check
    {
        return -1;
    }
    if (message == nullptr) //Simpler checks should be done first
    {
        return -1;
    }

    byte blockAddr4 = 4; //Non constant values used as constant. Seems to be a bit of a trend
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
    int stringLength2 = strlen(inputString2); //Maybe check if this does not exceed the size of bufferSize2
    byte dataBuffer[bufferSize] = "";
    byte dataBuffer2[bufferSize2] = "";

    // write data from buffer block Addr 4
    for (int j = 0; j < stringLength; j++) //j is non meaningfull, should have a better name probably
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
    byte trailerBlock = 7; //Constexpr
    // Serial.println(F("Authenticating again using key B..."));
    
    //Static cast please
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
