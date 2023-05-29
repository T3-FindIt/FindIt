#ifndef MFRC522WRITER_H
#define MFRC522WRITER_H

// #include <iostream>

#include <Arduino.h>
#include "IWriter.h"
#include <MFRC522.h>
#include <SPI.h>

class MFRC522Writer : public IWriter
{
private:
#define RST_PIN 9 // Configurable, see typical pin layout above
#define SS_PIN 10 // Configurable, see typical pin layout above
    MFRC522 mfrc522;
    MFRC522::MIFARE_Key key;
    const unsigned int MAX_MESSAGE_LENGTH = 29;
    const int BufferSize = 16;
    const int BufferSize2 = 12;
    char message[30];

    int dump_byte_array(byte *buffer, byte bufferSize);
    int authforwrite();
    int readTextFromSerial();
    int printStringData(byte *buffer, byte bufferSize);

public:
    MFRC522Writer(MFRC522 &nfc);
    void begin();
    int Transfer(byte *buffer, byte bufferSize);
    int Write();
};

#endif