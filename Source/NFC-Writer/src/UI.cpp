#include "UI.hpp"

UI::UI()
{
}

int UI::ReadTextFromSerial(char *message, int max_message_length)
{
    Serial.print("The max text is: ");
    Serial.print(max_message_length);
    Serial.println(" Characters");
    unsigned int message_pos = 0;
    message[0] = '\0';
    char messagetemp[1000] = "";
    messagetemp[0] = '\0';
    while (true)
    {
        if (Serial.available())
        {
            char incomingChar = Serial.read();
            // Serial.println(incomingChar);
            if (incomingChar == '\n')
            {
                // Enter key pressed, exit the loop
                int stringLength = strlen(messagetemp);
                for (int i = 0; i < stringLength; i++)
                {
                    if (i < max_message_length - 1)
                    {
                        message[i] = messagetemp[i];
                        message_pos = i + 1;
                    }
                    if (stringLength >= max_message_length)
                    {
                        message[0] = '\0';
                        Serial.print("The text limit is ");
                        Serial.print(max_message_length);
                        Serial.print(" Characters. Your text length is: ");
                        Serial.print(stringLength);
                        Serial.println(" Characters.");
                        Serial.println("Please try again ...");
                        Serial.println();
                        return -1;
                    }
                }
                if(strlen(message) == 0)
                {
                    message[0] = '\0';
                    Serial.println("Please try again. Empty text cannot be entered!");
                    Serial.println();
                    return -1;
                }
                message[message_pos] = '\0';
                messagetemp[0] = '\0';
                break;
            }
            // Append the incoming character to the text
            messagetemp[message_pos] = incomingChar;
            message_pos++;
        }
    }
    Serial.print("Received text: ");
    Serial.println(message);
    Serial.println("Keep the NFC-TAG on the NFC-Reader, then press Enter... ");
    while (true)
    {
        if (Serial.available())
        {
            char incomingChar = Serial.read();
            if (incomingChar == '\n')
            {
                break;
            }
        }
    }
    return 0;
}
