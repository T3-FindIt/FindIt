#ifndef UI_HPP
#define UI_HPP
#include <Arduino.h>

class UI
{
public:
    UI();
    ~UI() = default; //Good use of default.
    int ReadTextFromSerial(char *message, int max_message_length); //Why is this C style?

private:
};

#endif
