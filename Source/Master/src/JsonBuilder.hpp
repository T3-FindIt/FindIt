#include <ArduinoJson.h>

class JsonBuilder
{
public:
    JsonBuilder();

    bool Deserialize(std::string Json);
    bool Serialize(std::string actions[], int data[], int size, std::string &output);
};