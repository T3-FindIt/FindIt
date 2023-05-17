#include "JsonBuilder.hpp"
#include "json.hpp"
using json = nlohmann::json;

bool JsonBuilder::Deserialize(const std::string Json, std::string keys[], std::string values[], const int size)
{
    if (keys == nullptr || values == nullptr)
    {
        return false;
    }
    if (size <= 0)
    {
        return false;
    }

    if(Json.empty())
    {
        return false;
    }

    json deserializedJson = json::parse(Json);
    // This gets all the keys
    int keyPos = 0;
    for (json::iterator it = deserializedJson.begin(); it != deserializedJson.end(); ++it) 
    {
        keys[keyPos] = it.key();
        keyPos++;
    }

    int valuePos = 0;
    // This can get values from the json object
    for (json::iterator it = deserializedJson.begin(); it != deserializedJson.end(); ++it) 
    {
        values[valuePos] = it.value();
        valuePos++;
    }

    return true;
}

bool JsonBuilder::Serialize(const std::string keys[], const std::string values[], const int size, std::string &output)
{
    if (keys == nullptr || values == nullptr)
    {
        return false;
    }
    if (size <= 0)
    {
        return false;
    }

    json serializedJson;

    for (size_t i = 0; i < size; i++)
    {
        serializedJson[keys[i]] = values[i];
    }
    
    output = serializedJson.dump(); // Convert to string

    return true;
}
