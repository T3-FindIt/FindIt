#ifndef JSONBUILDER_HPP
#define JSONBUILDER_HPP

#include <string>


#define MAX_ARRAY_SIZE 10
#define ACTION_KEY "Action"
#define ACTION_INDEX 0

#define PAYLOAD_ONE 1
#define PAYLOAD_TWO 2
#define PAYLOAD_THREE 3

class JsonBuilder
{
public:
    JsonBuilder() = default;

    /// @brief Deserializes a JSON string into the given keys and values
    /// @param Json The input JSON string
    /// @param keys The keys to use
    /// @param values The values to use
    /// @param size The size of the keys and values arrays
    /// @return True if the deserialization was successful, false otherwise
    bool Deserialize(const std::string &Json, std::string keys[], std::string values[], const int size);

    /// @brief Serializes a JSON string from the given keys and values
    /// @param keys The keys to use
    /// @param values The values to use
    /// @param size The size of the keys and values arrays
    /// @param output The output string
    /// @return True if the serialization was successful, false otherwise
    bool Serialize(const std::string keys[], const std::string values[], const int size, std::string &output);
};

#endif // JSONBUILDER_HPP
