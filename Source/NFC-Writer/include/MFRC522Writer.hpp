#ifndef MFRC522WRITER_HPP
#define MFRC522WRITER_HPP

#include "IWriter.hpp"
#include <Arduino.h>
#include <MFRC522.h>
#include <SPI.h>

class MFRC522Writer : public IWriter
{

public:
    MFRC522Writer(MFRC522 &nfc);
    ~MFRC522Writer();
    void Begin();
    int Transfer(byte *buffer, byte bufferSize);
    int Write(char *message);

private:
    MFRC522 mfrc522;
    MFRC522::MIFARE_Key key;
    unsigned int maxMessageLength = 29;
    int bufferSize = 16;
    int bufferSize2 = 12;
    int DumpByteArray(byte *buffer, byte bufferSize);
    int AuthForWrite();
    int PrintStringData(byte *buffer, byte bufferSize);
    bool GetCardAvailable();
};

#endif
