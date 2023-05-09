#include <Arduino.h>
#include "I2CCommunication.hpp"

I2CCommunication* comToHub;
void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    comToHub = new I2CCommunication();
}

void loop()
{
    static char sendstring[49] = "HELL435834758937458374895789O";
    comToHub->SendNewItemToHub(sendstring);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
    
}
