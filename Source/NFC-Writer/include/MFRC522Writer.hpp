#ifndef MFRC522WRITER_HPP
#define MFRC522WRITER_HPP

#include "IWriter.hpp"
#include <Arduino.h>
#include <MFRC522.h>
#include <SPI.h>

class MFRC522Writer : public IWriter
{

public:
    MFRC522Writer(MFRC522 &nfc); // & and * should be on the variable type not the variable name. This is an old unused standard
    ~MFRC522Writer();
    void Begin();
    int Transfer(byte *buffer, byte bufferSize); //Buffersize can only ever be 255 max?
    int Write(char *message); // Why a C string?? Just use std::string

private:
    MFRC522& mfrc522; //Lifetime of this lives with the writer. This code cannot be tested without this object
    MFRC522::MIFARE_Key key;
    unsigned int maxMessageLength = 29; //Non constant used as constant. Use constexpr instead in the cpp file
    int bufferSize = 16;
    int bufferSize2 = 12; //This name tells me nothing
    int DumpByteArray(byte *buffer, byte bufferSize);
    int AuthForWrite(); //Why shorten this name?
    int PrintStringData(byte *buffer, byte bufferSize); //Print to what? //CString???
    bool GetCardAvailable();
};

#endif
