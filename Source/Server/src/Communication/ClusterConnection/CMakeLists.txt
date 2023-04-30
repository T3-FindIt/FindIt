cmake_minimum_required(VERSION ${CMakeMinimumVersion})

set(Headers
    IClusterConnection.hpp
    TCPClusterConnection.hpp
)

set(Sources
    TCPClusterConnection.cpp
)

add_library(ClusterConnection STATIC ${Sources} ${Headers})
