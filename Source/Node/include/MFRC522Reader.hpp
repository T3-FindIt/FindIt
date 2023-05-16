#ifndef MFRC522READER_HPP
#define MFRC522READER_HPP

#include <MFRC522.h>

#include "IReader.hpp"

class MFRC522Reader : public IReader
{
    private:
    MFRC522 reader;
    MFRC522::MIFARE_Key key;
    bool AuthenticateCard(MFRC522::StatusCode status);
    public:
    MFRC522Reader();
    MFRC522Reader(int SS_PIN, int RST_PIN);
    ~MFRC522Reader();
    bool CheckForCard();
    std::string ReadCard();
};

#endif // MFC522READER_HPP
