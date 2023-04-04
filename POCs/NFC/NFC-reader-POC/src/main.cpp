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

	Serial.println(F("Scan a MIFARE Classic PICC to demonstrate reading to string"));
	Serial.print(F("Using key (for A and B):"));
	dump_byte_array(key.keyByte, MFRC522::MF_KEY_SIZE);
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
	byte trailerBlock = 7;
	MFRC522::StatusCode status;


	// Authenticate using key A
	Serial.println(F("Authenticating using key A..."));
	status = (MFRC522::StatusCode)mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
	if (status != MFRC522::STATUS_OK)
	{
		Serial.print(F("PCD_Authenticate() failed: "));
		Serial.println(mfrc522.GetStatusCodeName(status));
		return;
	}

	// READING SECTOR 1 - COEN

	char OutputString[49];

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
				{
					if (readBuffer[j] == '\n')
					{
						OutputString[i*16+j] = '\0';
					}
					else
					{
						OutputString[i*16+j] = readBuffer[j];
					}
				}
			}
		}
	}

	Serial.println("output is:");
	Serial.println(OutputString);


	// Halt PICC
	mfrc522.PICC_HaltA();
	// Stop encryption on PCD
	mfrc522.PCD_StopCrypto1();
}
