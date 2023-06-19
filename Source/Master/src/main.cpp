#include <Arduino.h>

#include "I2C.hpp"
#include "WiFiHandler.hpp"
#include "WebSocketHandler.hpp"
#include "JsonBuilder.hpp"

#define LOCAL_ADDRESS 8 // slave address

// Might be better to put this in a secret.h file. To be discussed with the Git Master
#define SSID "SSID"
#define PASSWORD "PASSWORD"

#define SERVER_ADDRESS "SERVER IP"
#define SERVER_PORT 80

#define WIFI

WiFiHandler wifiHandler = WiFiHandler();
WebSocketHandler webSocketHandler = WebSocketHandler();
I2C i2c (LOCAL_ADDRESS);
JsonBuilder jsonBuilder;

#define SCAN_OFFSET 200 * 1000 // 10 Seconds
int scanTime = 0;

void setup()
{
    Serial.begin(9600);
    #ifdef WIFI
    WiFiData wifiData(SSID, PASSWORD);
    wifiHandler = WiFiHandler(wifiData);

    WebSocketData webSocketData(SERVER_ADDRESS, SERVER_PORT);
    webSocketHandler = WebSocketHandler(webSocketData);

    wifiHandler.Connect();
    if (!wifiHandler.isConnected())
    {
        Serial.println("Couldn't connect to the network!");
        return;
    }

    webSocketHandler.Connect();
    if (!webSocketHandler.isConnected())
    {
        Serial.println("Couldn't connect to the server!");
        return;
    }
    #endif
    Serial.println();
    Serial.println("Starting up!");
    #ifdef I2C_
    scanTime = millis() + SCAN_OFFSET;
    i2c.InitializeAddresses();
    #endif
}

int lastRequest;

// HANDLES ALL THE MESSAGES INCOMING FROM THE SERVER

bool Handle_Json(std::string keys[MAX_ARRAY_SIZE], std::string values[MAX_ARRAY_SIZE]) // TODO: This is a bit of a mess. Cleanup at some point. Turn this into a function pointer array?
{
    // Check if there is an actual request
    if(keys[ACTION_INDEX] == ACTION_KEY)
    {
        return false;
    }

    if(keys[ACTION_INDEX] == "HeartBeat") // For the HeartBeat
    {
        std::string firstPayload = "Demo_Node"; // Testing name. We never actually decided on anything
        std::string secondPayload = std::to_string(i2c.GetNodeCount()); // Get count

        // Protocol Compliant
        keys[PAYLOAD_ONE] = "Node";
        keys[PAYLOAD_TWO] = "Places";

        // Send the values
        values[PAYLOAD_ONE] = firstPayload;
        values[PAYLOAD_TWO] = secondPayload;

        std::string json;
        if(!jsonBuilder.Serialize(keys,values, TWO_PAYLOAD_ELEMENTS, json)) // Parse!
        {
            return false;
        }

        webSocketHandler.Send(json); // And send it.
        return true;
    }

    if(keys[ACTION_INDEX] == "RequestProduct")
    {
        std::string firstPayload = "ResponseProduct";
        #ifdef I2C_
        std::string secondPayload = i2c.GetProductName(values[PAYLOAD_ONE]);
        #endif
        std::string secondPayload = "Demo_Product"; // Code for getting the product name goes here. This also activates the corresponding node :)

        // Protocol Compliant
        keys[PAYLOAD_ONE] = "Action";
        keys[PAYLOAD_TWO] = "ProductName";
        keys[PAYLOAD_THREE] = "Result";

        // Send the values
        values[PAYLOAD_ONE] = firstPayload;
        values[PAYLOAD_TWO] = secondPayload;
        values[PAYLOAD_THREE] = "true"; // TODO: Change the product status to the actual status

        std::string json;
        if(!jsonBuilder.Serialize(keys,values, MAX_ELEMENTS, json)) // Parse!
        {
            return false;
        }

        webSocketHandler.Send(json); // And send it.
        return true;
    }

    if(keys[0] == "NotifyNewProduct")
    {
        return true;
    }
    
    return false;
}

void loop()
{
    #ifdef WIFI
    if (webSocketHandler.isConnected())
    {
        std::string websocketData = webSocketHandler.Recieve();
        if (websocketData != "")
        {
            std::string keys[MAX_ARRAY_SIZE];
            std::string values[MAX_ARRAY_SIZE];
            if(jsonBuilder.Deserialize(websocketData, keys, values, MAX_ARRAY_SIZE))
            {
                #ifdef SERIAL_DEBUG
                Serial.println(websocketData.c_str());
                #endif

                if(Handle_Json(keys, values) == false)
                {
                    #ifdef SERIAL_DEBUG
                    Serial.println("Failed to detect message.");
                    #endif
                    return;
                }
            }
        }
        #ifdef SERIAL_DEBUG
        Serial.println("Somethign went wrong with the deserialisation.");
        #endif
    }
    #endif

    #ifdef I2C_
    if (i2c.IsAvailable())
    {
        if (i2c.GetLastChange() == LAST_ITEM_REGISTER)
        {
            Serial.print("Last address:");
            Serial.println(i2c.GetLastAddress());
            char data[MAX_STRING_SIZE];
            i2c.GetRegister(LAST_ITEM_REGISTER,&data[0]);
            Serial.print("Item: ");
            Serial.println(data);
        }
    }

    if (scanTime < millis())
    {
        i2c.Scan();
        scanTime = millis() + SCAN_OFFSET;
    }
    #endif
}
