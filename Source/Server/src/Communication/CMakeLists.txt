cmake_minimum_required(VERSION ${CMakeMinimumVersion})

set(Headers
    Communication.hpp
)

set(Sources
    Communication.cpp
)

add_library(Communication STATIC ${Sources} ${Headers})

target_include_directories(Communication PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Communication PRIVATE
MessageInterface
ProtocolParser
ClusterConnection
)

add_subdirectory(ProtocolParser)
add_subdirectory(ClusterConnection)
