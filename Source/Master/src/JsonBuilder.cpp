#include "JsonBuilder.hpp"
#include <string>

JsonBuilder::JsonBuilder()
{
}

bool JsonBuilder::Deserialize(std::string Json)
{

    if(Json == "")
    {
        return false;
    }

    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, Json);
    if (error)
    {
        return false;
    }

    std::string action = doc["Action"];

    if(strcmp(action.c_str(), "HeartBeat") != 0)
    {
        return false;
    }

    return true;

}

bool JsonBuilder::Serialize(std::string actions[], int data[], int size, std::string &output)
{
    if(actions == NULL || data == NULL || size <= 0)
    {
        return false;
    }

    StaticJsonDocument<200> doc;

    for (size_t i = 0; i < size; i++)
    {
        /* code */
    }
    

    return false;
}
