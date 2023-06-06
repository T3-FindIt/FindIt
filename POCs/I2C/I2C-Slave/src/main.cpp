#include <Arduino.h>
#include <Wire.h>

#define SLAVE_ADDRESS 0x4
#define SENT_DATA 69
#define EXPECTED_BYTES 16

void requestData() {
    for (size_t i = 0; i < EXPECTED_BYTES; i++)
    {
        Wire.write(SENT_DATA);
    }
}

void setup() {
    Wire.begin(SLAVE_ADDRESS);
    Wire.onRequest(requestData);
}

void loop() {
  // put your main code here, to run repeatedly:
}
