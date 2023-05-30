#ifndef UI_HPP
#define UI_HPP
#include <Arduino.h>

class UI
{
public:
    UI();
    ~UI() = default;
    int ReadTextFromSerial(char *message, int max_message_length);

private:
};

#endif
