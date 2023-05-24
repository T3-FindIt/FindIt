cmake_minimum_required(VERSION ${CMakeMinimumVersion})

set(Headers
    IClusterConnection.hpp
    TCPConnection.hpp
    Exceptions/FailedSocketInit.hpp
    Exceptions/FailedWSAStartup.hpp
)

set(Sources
    TCPConnection.cpp
)

add_library(ClusterConnection STATIC ${Sources} ${Headers})

if(WIN32)
    target_link_libraries(ClusterConnection PRIVATE wsock32 ws2_32)
endif()

target_include_directories(ClusterConnection PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
