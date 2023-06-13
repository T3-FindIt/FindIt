#ifndef UARTINTERFACE_HPP
#define UARTINTERFACE_HPP
#include <Arduino.h>

class UARTI
{
public:
    UARTI();
    ~UARTI();
    int ReadTextFromSerial(char* message, int max_message_length);

private:
char* inputBuffer;
int bufferIndex;
bool storeText;

};

#endif
