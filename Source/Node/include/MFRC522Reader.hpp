#ifndef MFRC522READER_HPP
#define MFRC522READER_HPP

#define DEFAULT_SS_PIN 10
#define DEFAULT_RST_PIN 9

#include <MFRC522.h>

#include "IReader.hpp"

//At first glance this looks to have a lot in common with the NFCWriter from the other branch. 
//Please consolidate with an abstract base class
class MFRC522Reader : public IReader
{
private:
    MFRC522 reader;
    MFRC522::MIFARE_Key key;
    bool AuthenticateCard(); 
public:
    MFRC522Reader() : MFRC522Reader(DEFAULT_SS_PIN, DEFAULT_RST_PIN) {}
    MFRC522Reader(int SS_PIN, int RST_PIN);
    ~MFRC522Reader();
    bool CheckForCard();
    // outputstring should be [49]
    MFRC522::StatusCode status;
    int ReadCard(char* outputString, int arrayLength);
};

#endif // MFRC522READER_HPP
