cmake_minimum_required(VERSION ${CMakeMinimumVersion})

set(Headers
    IClusterConnection.hpp
    TCPClusterConnection.hpp
)

set(Sources
    TCPClusterConnection.cpp
)

set(GlobalIncludeDirs
    ${GlobalIncludeDirs}
    ${CMAKE_CURRENT_LIST_DIR}
    PARENT_SCOPE
)

add_library(ClusterConnection STATIC ${Sources} ${Headers})
