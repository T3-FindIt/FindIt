#ifndef JSONBUILDER_HPP
#define JSONBUILDER_HPP

#include <string>

class JsonBuilder
{
public:
    JsonBuilder() = default; //Nice

    //I love doxyen style comments
    //However, if it isn't done everywhere in the code and/or updating them isn't a part of the Way of Working for a pr
    //They tend to become hassles more than helpful fairly quickly.
    //So I advice against them.

    /// @brief Deserializes a JSON string into the given keys and values
    /// @param Json The input JSON string
    /// @param keys The keys to use //Especially when they're wrong. This is clearly an out variable.
    /// @param values The values to use //Same here
    /// @param size The size of the keys and values arrays
    /// @return True if the deserialization was successful, false otherwise
    bool Deserialize(const std::string &Json, std::string keys[], std::string values[], const int size);

    /// @brief Serializes a JSON string from the given keys and values
    /// @param keys The keys to use
    /// @param values The values to use
    /// @param size The size of the keys and values arrays
    /// @param output The output string //Out variable
    /// @return True if the serialization was successful, false otherwise
    bool Serialize(const std::string keys[], const std::string values[], const int size, std::string &output);
};

#endif // JSONBUILDER_HPP
