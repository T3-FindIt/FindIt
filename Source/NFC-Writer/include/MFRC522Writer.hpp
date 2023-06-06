#ifndef MFRC522WRITER_HPP
#define MFRC522WRITER_HPP

#include "IWriter.hpp"
#include <Arduino.h>
#include <MFRC522.h>
#include <SPI.h>

class MFRC522Writer : public IWriter
{
public:
    MFRC522Writer(MFRC522& nfc);
    ~MFRC522Writer();
    void Begin();
    int Transfer(byte* buffer, byte blockAddr, int bufferSize);
    int Write(char* message);

private:
    MFRC522& mfrc522;
    MFRC522::MIFARE_Key key;
    int AuthenticateWrite();
    bool GetCardAvailable();
};

#endif
