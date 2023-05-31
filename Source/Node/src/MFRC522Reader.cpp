#include "MFRC522Reader.hpp"
#include <SPI.h>

#define DEFAULT_KEY 0xFF //Don't use defines please, use constexpr
#define MAX_STRING_ARRAY_LENGTH 49
#define LAST_STRING_ARRAY_BYTE 48
#define MIFARE_BUFFER_SIZE 18
#define TAG_START_ADRESS 4
#define RESET_BUFFER_TIME 50
#define NFC_TAG_BLOCK_SIZE 16
#define DEFAULT_TRAILER_BLOCK 7
#define KEY_SIZE 6


MFRC522Reader::MFRC522Reader(int SS_PIN, int RST_PIN)
{
    reader = MFRC522(SS_PIN, RST_PIN); //This lives with this object.
    //I believe you are implicitely making this object not copyable or maybe even a singleton like object.
    //What would happen if I were to create another of this object?
    //Prevent this or fix
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
    //Static cast please
    status = (MFRC522::StatusCode)reader.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, DEFAULT_TRAILER_BLOCK, &key, &(reader.uid));
    
    //Better to do the following:
    //This is more extendable. Up to you all to figure out why ;)
    if (status == MFRC522::STATUS_OK)
    {
        return true;
    }
    return false;
}

bool MFRC522Reader::CheckForCard() //This looks a LOT like the function in the writer. Share code?
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



// outputstring has to be [49]
int MFRC522Reader::ReadCard(char* outputString, int arrayLength) //C strings are baaaddd. Only the lowest part of the code should deal with them.
{
    if (arrayLength < MAX_STRING_ARRAY_LENGTH)
    {
        return -1;
    }
    MFRC522::PICC_Type piccType = reader.PICC_GetType(reader.uid.sak);

    //More extendable way of doing this is detailed below
    if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI 
    && piccType != MFRC522::PICC_TYPE_MIFARE_1K 
    && piccType != MFRC522::PICC_TYPE_MIFARE_4K)
    {
        return -1;
    }

    if (!AuthenticateCard())
    {
        return -1;
    }

    for (int i = 2; i >= 0; i--) //Magic numberrs
    {
        byte readBuffer[MIFARE_BUFFER_SIZE];
        byte size = sizeof(readBuffer);

        byte address = TAG_START_ADRESS + i;
        status = (MFRC522::StatusCode)reader.MIFARE_Read(address, readBuffer, &size);
        //Looks good
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
    //Convert outputstring to std::string and return that.
    return 0;
}

//Extendable, with proper constants of course. No clue if this compiles
std::set<MFRC522::PICC_Type> UnsupportedPiccs =
{
    MFRC522::PICC_TYPE_MIFARE_MINI,
    MFRC522::PICC_TYPE_MIFARE_1K,
    MFRC522::PICC_TYPE_MIFARE_4K
};

bool piccSupported(const MFRC522::PICC_Type& picc)
{
    //Find might need a compare to work. If so, use the concept in another way.
    //Most important part is to represent your unsupported piccs as data instead of code
    //And then use the data as logic input.
    //To add more unsupported piccs, you just have to change the set instead of having to change the code.
    if (auto search = UnsupportedPiccs.find(picc); search != UnsupportedPiccs.end()) 
        return false; //Found
    else
        return true; //Not found
}
