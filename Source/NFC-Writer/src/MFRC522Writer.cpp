#include "MFRC522Writer.h"

MFRC522Writer::MFRC522Writer(MFRC522 &nfc)
{
    mfrc522 = nfc;
}

void MFRC522Writer::begin()
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

    byte blockAddr4 = 4;
    byte blockAddr5 = 5;
    byte trailerBlock = 7;
    MFRC522::StatusCode status;
    byte buffer[30] = "";
    byte buffer4[18] = "";
    byte buffer5[14] = "";
    byte size = sizeof(buffer4);

    // Serial.println(F("Authenticating using key A..."));
    status = (MFRC522::StatusCode)mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK)
    {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return -1;
    }

    // read data from buffer block Addr 4
    status = (MFRC522::StatusCode)mfrc522.MIFARE_Read(blockAddr4, buffer4, &size);
    if (status != MFRC522::STATUS_OK)
    {
        Serial.print(F("MIFARE_Read() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
    }

    // read data from buffer block Addr 5
    status = (MFRC522::StatusCode)mfrc522.MIFARE_Read(blockAddr5, buffer5, &size);
    if (status != MFRC522::STATUS_OK)
    {
        Serial.print(F("MIFARE_Read() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
    }

    // Consolidate the data into a one buffer
    for (int i = 0; i < BufferSize; i++)
    {
        buffer[i] = buffer4[i];
    }
    for (int i = 0; i < BufferSize2; i++)
    {
        buffer[i + BufferSize] = buffer5[i];
    }

    Serial.print("Current data: ");
    printStringData(buffer, MAX_MESSAGE_LENGTH - 1);
    Serial.println();
    Serial.println();

    // Serial.println(F("Authenticating again using key B..."));
    status = (MFRC522::StatusCode)mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, trailerBlock, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK)
    {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return -1;
    }

    Serial.println("Writing yuor string Data: ...");
    readTextFromSerial();
    char inputString[MAX_MESSAGE_LENGTH] = "";
    char inputString2[MAX_MESSAGE_LENGTH] = "";
    int messageLength = strlen(message);
    for (int i = 0; i < messageLength; i++)
    {
        if (i < BufferSize)
        {
            inputString[i] = message[i];
        }
        else if (i >= BufferSize)
        {
            inputString2[i - BufferSize] = message[i];
        }
    }

    int stringLength = strlen(inputString);
    int stringLength2 = strlen(inputString2);
    byte dataBuffer[BufferSize] = "";
    byte dataBuffer2[BufferSize2] = "";

    // write data from buffer block Addr 4
    for (int j = 0; j < stringLength; j++)
    {
        dataBuffer[j] = inputString[j];
    }
    if (stringLength < BufferSize)
    {
        for (int j = 0; j < BufferSize - stringLength; j++)
        {
            dataBuffer[stringLength + j] = 0;
        }
    }

    // write data from buffer block Addr 5
    for (int j = 0; j < stringLength2; j++)
    {
        dataBuffer2[j] = inputString2[j];
    }
    if (stringLength2 < BufferSize2)
    {
        for (int j = 0; j < BufferSize2 - stringLength2; j++)
        {
            dataBuffer2[stringLength2 + j] = 0;
        }
    }

    if (authforwrite() == -1)
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

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    return 0;
}

int MFRC522Writer::dump_byte_array(byte *buffer, byte bufferSize)
{
    if (buffer == nullptr || bufferSize == 0)
    {
        return -1;
    }
    for (byte i = 0; i < bufferSize; i++)
    {
        Serial.print(buffer[i] < 0x10 ? " 0" : "");
        Serial.print(buffer[i], HEX);
    }
    return 0;
}

int MFRC522Writer::printStringData(byte *buffer, byte bufferSize)
{
    if (buffer == nullptr || bufferSize == 0)
    {
        return -1;
    }
    for (int i = 0; i < bufferSize; i++)
    {
        char c = char(buffer[i]);
        if (c != 0)
        {
            Serial.print(c);
        }
    }
    return 0;
}
int MFRC522Writer::authforwrite()
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
    MFRC522::StatusCode status = (MFRC522::StatusCode)mfrc522.MIFARE_Write(blockAddr, inputBlock, BufferSize);
    if (status != MFRC522::STATUS_OK)
    {
        Serial.print(F("MIFARE_Write() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return -1;
    }
    Serial.println();
    return 0;
}

int MFRC522Writer::readTextFromSerial()
{
    unsigned int message_pos = 0;
    message[0] = '\0';
    char messagetemp[100] = "";
    messagetemp[0] = '\0';
    while (true)
    {
        if (Serial.available())
        {
            char incomingChar = Serial.read();
            if (incomingChar == '\n')
            {
                // Enter key pressed, exit the loop
                unsigned int stringLength = strlen(messagetemp);
                for (unsigned int i = 0; i < MAX_MESSAGE_LENGTH - 1; i++)
                {
                    /* code */
                    if (i < stringLength)
                    {
                        message[i] = messagetemp[i];
                        message_pos = i + 1;
                    }
                }

                message[message_pos] = '\0';
                messagetemp[0] = '\0';
                break;
            }

            // Append the incoming character to the text
            messagetemp[message_pos] = incomingChar;
            message_pos++;
        }
    }
    Serial.print("Received text: ");
    Serial.println(message);
    return 0;
}