cmake_minimum_required(VERSION ${CMakeMinimumVersion})

# Add headers here
set(Headers
    IProtocolParser.hpp
    JSONProtocolParser.hpp
)

# Add sources here
set(Sources
    JSONProtocolParser.cpp
)

# Makes a library from the sources and headers. First argument is the name of the library (used in src/CMakeLists.txt).
add_library(ProtocolParser STATIC ${Sources} ${Headers})

target_link_libraries(ProtocolParser PRIVATE nlohmann_json::nlohmann_json)
