#include <Arduino.h>
#include <MFRC522.h>
#include <SPI.h>
/*
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        3                10
 * IRQ         ?            ?             ?         ?          2                10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 *
 */

#define RST_PIN 9 // Configurable, see typical pin layout above
#define SS_PIN 10 // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

void dump_byte_array(byte *buffer, byte bufferSize)
{
	for (byte i = 0; i < bufferSize; i++)
	{
		Serial.print(buffer[i] < 0x10 ? " 0" : " ");
		Serial.print(buffer[i], HEX);
	}
}

void setup()
{
	// put your setup code here, to run once:
	Serial.begin(9600); // Initialize serial communications with the PC
	while (!Serial)
		;				// Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
	SPI.begin();		// Init SPI bus
	mfrc522.PCD_Init(); // Init MFRC522 card

	// Prepare the key (used both as key A and as key B)
	// using FFFFFFFFFFFFh which is the default at chip delivery from the factory
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

void authforwrite()
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
void writeBlockToFirstSector(byte* inputBlock, byte blockAddr)
{
	MFRC522::StatusCode status = (MFRC522::StatusCode)mfrc522.MIFARE_Write(blockAddr, inputBlock, 16);
	if (status != MFRC522::STATUS_OK)
	{
		Serial.print(F("MIFARE_Write() failed: "));
		Serial.println(mfrc522.GetStatusCodeName(status));
	}
	Serial.println();
}

void loop()
{

	if (!mfrc522.PICC_IsNewCardPresent())
		return;
	// Select one of the cards
	if (!mfrc522.PICC_ReadCardSerial())
		return;

	// Show some details of the PICC (that is: the tag/card)
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
		return;
	}

	// In this sample we use the second sector,
	// that is: sector #1, covering block #4 up to and including block #7
	byte sector = 1;
	byte blockAddr = 4;
	byte dataBlock[] = {
		0x01, 0x02, 0x03, 0x04, //  1,  2,   3,  4,
		0x05, 0x06, 0x07, 0x08, //  5,  6,   7,  8,
		0x09, 0x0a, 0xff, 0x0b, //  9, 10, 255, 11,
		0x0c, 0x0d, 0x0e, 0x0f	// 12, 13, 14, 15
	};
	byte trailerBlock = 7;
	MFRC522::StatusCode status;
	byte buffer[18];
	byte size = sizeof(buffer);

	// Authenticate using key A
	Serial.println(F("Authenticating using key A..."));
	status = (MFRC522::StatusCode)mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
	if (status != MFRC522::STATUS_OK)
	{
		Serial.print(F("PCD_Authenticate() failed: "));
		Serial.println(mfrc522.GetStatusCodeName(status));
		return;
	}

	// Show the whole sector as it currently is
	Serial.println(F("Current data in sector:"));
	mfrc522.PICC_DumpMifareClassicSectorToSerial(&(mfrc522.uid), &key, sector);
	Serial.println();

	// Read data from the block
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

	// Authenticate using key B
	Serial.println(F("Authenticating again using key B..."));
	status = (MFRC522::StatusCode)mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, trailerBlock, &key, &(mfrc522.uid));
	if (status != MFRC522::STATUS_OK)
	{
		Serial.print(F("PCD_Authenticate() failed: "));
		Serial.println(mfrc522.GetStatusCodeName(status));
		return;
	}

	// Write data to the block
	// Serial.print(F("Writing data into block "));
	// Serial.print(blockAddr);
	// Serial.println(F(" ..."));
	// dump_byte_array(dataBlock, 16);
	// Serial.println();
	status = (MFRC522::StatusCode)mfrc522.MIFARE_Write(blockAddr, dataBlock, 16);
	if (status != MFRC522::STATUS_OK)
	{
		Serial.print(F("MIFARE_Write() failed: "));
		Serial.println(mfrc522.GetStatusCodeName(status));
	}
	Serial.println();




	//WRITING STRING TO SECTOR 0 AND ON - COEN
	Serial.println();
	Serial.println();
	Serial.println("Writing string to sector 0");

	char inputString[49] = "this is the max stringsize 12345678901234567890\n";
	int stringLength = strlen(inputString);
	for (int i = 0; i < stringLength; i++)
	{
		byte address = i/16+4;
		int blockSize;
		if (stringLength-i >= 16)
		{
			blockSize = 16;
		}
		else
		{
			blockSize = stringLength%16;
		}
		byte dataBuffer[16];
		
		for (int j = 0; j < blockSize; j++)
		{
			dataBuffer[j] = inputString[i+j];
		}
		if (blockSize < 16)
		{
			for (int j = 0; j < 16-blockSize; j++)
			{
				dataBuffer[blockSize+j] = 0;
			}
		}
		authforwrite();
		writeBlockToFirstSector(dataBuffer, address);
		i = i+blockSize-1;
	}


	// Dump the sector data
	Serial.println(F("Current data in sector:"));
	mfrc522.PICC_DumpMifareClassicSectorToSerial(&(mfrc522.uid), &key, sector);
	Serial.println();

	// Halt PICC
	mfrc522.PICC_HaltA();
	// Stop encryption on PCD
	mfrc522.PCD_StopCrypto1();
}