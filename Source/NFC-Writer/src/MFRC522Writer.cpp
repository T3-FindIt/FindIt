
#include "MFRC522Writer.h"


MFRC522Writer::MFRC522Writer(MFRC522 &nfc)
{
    mfrc522 = nfc;
}

int MFRC522Writer::Transfer(byte data)
{
    return 0;
}

void MFRC522Writer::begin()
{
    //Serial.begin(9600);
    while (!Serial)
    {

    }
    SPI.begin();
    mfrc522.PCD_Init();

    for (byte i = 0; i < 6; i++)
    {
        key.keyByte[i] = 0xFF;
    }

    Serial.println(F("Scan a MIFARE Classic PICC to demonstrate read and write."));
    Serial.print(F("Using key (for A and B):"));
    dump_byte_array(key.keyByte, MFRC522::MF_KEY_SIZE);
    Serial.println();

    Serial.println(F("BEWARE: Data will be written to the PICC, in sector #1"));
}

int MFRC522Writer::Write()
{
    if (!mfrc522.PICC_IsNewCardPresent())
    {
        return -1;
    }
    if (!mfrc522.PICC_ReadCardSerial())
    {
        return -1;
    }

    Serial.print(F("Card UID:"));
    dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
    Serial.println();
    Serial.print(F("PICC type: "));
    MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    Serial.println(mfrc522.PICC_GetTypeName(piccType));

    // Check for compatibility
    if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI && piccType != MFRC522::PICC_TYPE_MIFARE_1K && piccType != MFRC522::PICC_TYPE_MIFARE_4K)
    {
        Serial.println(F("This sample only works with MIFARE Classic cards."));
        return -1;
    }

    byte sector = 1;
    byte blockAddr = 4;
    byte trailerBlock = 7;
    MFRC522::StatusCode status;
    byte buffer[18];
    byte size = sizeof(buffer);

    Serial.println(F("Authenticating using key A..."));
    status = (MFRC522::StatusCode)mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK)
    {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return -1;
    }

    Serial.println(F("Current data in sector:"));
    mfrc522.PICC_DumpMifareClassicSectorToSerial(&(mfrc522.uid), &key, sector);
    Serial.println();

    Serial.print(F("Reading data from block "));
    Serial.print(blockAddr);
    Serial.println(F(" ..."));
    status = (MFRC522::StatusCode)mfrc522.MIFARE_Read(blockAddr, buffer, &size);
    if (status != MFRC522::STATUS_OK)
    {
        Serial.print(F("MIFARE_Read() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
    }
    Serial.print(F("Data in block "));
    Serial.print(blockAddr);
    Serial.println(F(":"));
    dump_byte_array(buffer, 16);
    Serial.println();
    Serial.println();

    Serial.println(F("Authenticating again using key B..."));
    status = (MFRC522::StatusCode)mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, trailerBlock, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK)
    {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return -1;
    }

    // WRITING STRING TO SECTOR 1 AND ON - COEN
    Serial.println();
    Serial.println();
    Serial.println("Writing string to sector 0");
    while (strlen(message) == 0)
    {
        read();
    }
    char inputString[49] = "this is the max stringsize 12345678901234567890\n";
    int stringLength = strlen(inputString);
    for (int i = 0; i < stringLength; i++)
    {
        byte address = i / 16 + 4;
        int blockSize;
        if (stringLength - i >= 16)
        {
            blockSize = 16;
        }
        else
        {
            blockSize = stringLength % 16;
        }
        byte dataBuffer[16];

        for (int j = 0; j < blockSize; j++)
        {
            dataBuffer[j] = inputString[i + j];
        }
        if (blockSize < 16)
        {
            for (int j = 0; j < 16 - blockSize; j++)
            {
                dataBuffer[blockSize + j] = 0;
            }
        }
        authforwrite();
        writeBlockToFirstSector(dataBuffer, address);
        i = i + blockSize - 1;
    }

    Serial.println(F("=========== [SUCCESS] ==========="));
    print();
    Serial.println(F("Current data in sector:"));
    mfrc522.PICC_DumpMifareClassicSectorToSerial(&(mfrc522.uid), &key, sector);
    Serial.println();

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    return 0;
}


void  MFRC522Writer::dump_byte_array(byte *buffer, byte bufferSize)
{
    for (byte i = 0; i < bufferSize; i++)
    {
        Serial.print(buffer[i] < 0x10 ? " 0" : "");
        Serial.print(buffer[i], HEX);
    }
}

void MFRC522Writer::authforwrite()
{
    byte trailerBlock = 7;
    Serial.println(F("Authenticating again using key B..."));
    MFRC522::StatusCode status = (MFRC522::StatusCode)mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, trailerBlock, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK)
    {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    }
}

void MFRC522Writer::writeBlockToFirstSector(byte *inputBlock, byte blockAddr)
{
    MFRC522::StatusCode status = (MFRC522::StatusCode)mfrc522.MIFARE_Write(blockAddr, inputBlock, 16);
    if (status != MFRC522::STATUS_OK)
    {
        Serial.print(F("MIFARE_Write() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
    }
    Serial.println();
}

void MFRC522Writer::read()

{
    while (Serial.available() > 0)
   {
   //Create a place to hold the incoming message
   //message[MAX_MESSAGE_LENGTH];
   static unsigned int message_pos = 0;

   //Read the next available byte in the serial receive buffer
   char inByte = Serial.read();

   //Message coming in (check not terminating character) and guard for over message size
   if ( inByte != '\n' && (message_pos < MAX_MESSAGE_LENGTH - 1) )
   {
     //Add the incoming byte to our message
     message[message_pos] = inByte;
     message_pos++;
   }
   //Full message received...
   else
   {
     //Add null character to string
     message[message_pos] = '\0';

     //Print the message (or do other things)
    Serial.println(message);

     //Reset for the next message
     message_pos = 0;
   }
 }
}

void MFRC522Writer::print()
{
 Serial.println(message);
}