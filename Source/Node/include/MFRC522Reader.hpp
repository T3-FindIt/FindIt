#ifndef MFRC522READER_HPP
#define MFRC522READER_HPP

#include <MFRC522.h>

#include "IReader.hpp"

#define DEFAULT_SS_PIN 10
#define DEFAULT_RST_PIN 9
#define DEFAULT_KEY 0xFF
#define DEFAULT_TRAILER_BLOCK 7

class MFRC522Reader : public IReader
{
    private:
    MFRC522 reader;
    MFRC522::MIFARE_Key key;
    bool AuthenticateCard();
    public:
    MFRC522Reader();
    MFRC522Reader(int SS_PIN, int RST_PIN);
    ~MFRC522Reader();
    bool CheckForCard();
    // outputstring should be [49]
    MFRC522::StatusCode status;
    int ReadCard(char* outputString);
};

#endif // MFRC522READER_HPP
