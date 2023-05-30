#include <Arduino.h>
#include <Wire.h>

#define SLAVE_ADDRESS 0x4
#define ADDRESS_LIMIT 127
#define EXPECTED_DATA 69

#define REQUESTED_DATA_SIZE 16

int bytesRecieved = 0;

void OnRecieve(int bytes)
{
    int data = 0;
    data = Wire.read();
    if (data == EXPECTED_DATA)
    {
        bytesRecieved++;
    }
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

void setup()
{
  // put your setup code here, to run once:
    Wire.begin();
    Wire.onReceive(OnRecieve);
    Serial.begin(9600);
}

void loop()
{
  // put your main code here, to run repeatedly:
    int startTime = millis();
    for (size_t i = 0; i < ADDRESS_LIMIT; i++)
    {
        Wire.requestFrom(SLAVE_ADDRESS, REQUESTED_DATA_SIZE);
        byte buff[REQUESTED_DATA_SIZE];
        int recievedBytes = Wire.readBytes(buff, REQUESTED_DATA_SIZE); // read value of the 'result' register
        if (recievedBytes == REQUESTED_DATA_SIZE)
        {
            bytesRecieved++;
        }
    }
    int endTime = millis();

    Serial.print("Time: ");
    Serial.println(endTime - startTime);
    Serial.print("Successfully recieved:");
    Serial.println(bytesRecieved);

    bytesRecieved = 0;
    delay(1000);
}
