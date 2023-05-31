#include "JsonBuilder.hpp"
#include "json.hpp"
using json = nlohmann::json; //Please abstain from using this.

//This is a C-style function. It should return the objects and throw an exception if anything VERY wrong
//Otherwise you should just check the output to see if anything went wrong.
//I think returning an std::map here would work perfectly. 
//Or maybe just returning the json object??? Don't really see why not, as it seems to work like a map already.
bool JsonBuilder::Deserialize(const std::string &Json, std::string keys[], std::string values[], const int size)
{
    if (keys == nullptr || values == nullptr) //Would not be necessary if not done C-Style.
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

//Same comments as above.
//You seem to be translating these json objects into 2 arrays for no reason.
//They are already bundled nicely as keyValuePairs. Just keep them that way. No reason to do it in any other way.
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
    
    //Why not return this instead of an out variable?
    output = serializedJson.dump(); // Convert to string 

    return true;
}
