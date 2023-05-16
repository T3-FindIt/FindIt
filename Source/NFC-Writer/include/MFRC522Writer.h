#ifndef MFRC522WRITER_H
#define MFRC522WRITER_H

//#include <iostream>

#include <Arduino.h>
#include "IWriter.h"
#include <MFRC522.h>
#include <SPI.h>

class MFRC522Writer  : public IWriter
{
private:

#define RST_PIN 9 // Configurable, see typical pin layout above
#define SS_PIN 10 // Configurable, see typical pin layout above
MFRC522 mfrc522;
MFRC522::MIFARE_Key key;
const unsigned int MAX_MESSAGE_LENGTH = 47;
char message[48];

void dump_byte_array(byte *buffer, byte bufferSize);
void authforwrite();
void writeBlockToFirstSector(byte *inputBlock, byte blockAddr);
void read();
void print();



public:
   MFRC522Writer(MFRC522& nfc);
	int Transfer(byte data);
    int Write();
    void begin();
};

#endif