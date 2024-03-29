#include "UARTInterface.hpp"

constexpr int minCharArraySize = 2;

UARTI::UARTI() : inputBuffer(NULL), bufferIndex(0), storeText(false) {}

UARTI::~UARTI()
{
    delete (inputBuffer);
    inputBuffer = nullptr;
}

int UARTI::ReadTextFromSerial(char* message, int maxMessageLength)
{
    bool printMessage = false;
    if (Serial.available())
    {
        char incomingChar = Serial.read();
        if (incomingChar != '\n' || incomingChar != ' ')
        {
            storeText = true;
        }

        if (incomingChar == '\n' || incomingChar == '\r')
        {
            if (storeText)
            {
                if (bufferIndex > 0)
                {
                    inputBuffer[bufferIndex] = '\0'; // Add null terminator
                    printMessage = true;
                }
            }
            else
            {
                // Start storing the text
                storeText = true;
                inputBuffer = (char *)malloc(sizeof(char) * minCharArraySize); // Allocate memory for the input buffer
            }
        }
        else if (storeText)
        {
            // Expand the input buffer to accommodate the new character
            inputBuffer = (char *)realloc(inputBuffer, (bufferIndex + 1) * sizeof(char));
            inputBuffer[bufferIndex] = incomingChar; // Store the received character
            bufferIndex++;

        }
    }
    if (printMessage)
    {
        if (bufferIndex < maxMessageLength)
        {
            for (int i = 0; i < bufferIndex; i++)
            {
                message[i] = inputBuffer[i];
            }
        }
        else
        {
            Serial.print("The text limit is ");
            Serial.print(maxMessageLength - 1);
            Serial.print(" Characters. Your text length is: ");
            Serial.print(bufferIndex);
            Serial.println(" Characters.");
            Serial.println("Please try again ...");
            Serial.println();
            bufferIndex = 0;
            storeText = false;
            strcpy(message, "");
            return -1;
        }
        if (bufferIndex == 0)
        {
            Serial.println("Please try again. Empty text cannot be entered!");
            Serial.println();
            return -1;
        }
        else
        {
            Serial.print("Received text: ");
            Serial.println(message);
        }
        // Reset the buffer index and flag
        bufferIndex = 0;
        storeText = false;
        return 0;
    }
    return -1;
}
