cmake_minimum_required(VERSION ${CMakeMinimumVersion})

set(Headers
    IProtocolParser.hpp
    JSONProtocolParser.hpp
)

set(Sources
    JSONProtocolParser.cpp
)

add_library(ProtocolParser STATIC ${Sources} ${Headers})

target_link_libraries(ProtocolParser PRIVATE
    nlohmann_json::nlohmann_json
    MessageInterface
)

target_include_directories(ProtocolParser PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
