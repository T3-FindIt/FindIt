cmake_minimum_required(VERSION ${CMakeMinimumVersion})

# Add headers here
set(Headers
    IClusterConnection.hpp
    TCPClusterConnection.hpp
)

# Add sources here
set(Sources
    TCPClusterConnection.cpp
)

# Makes a library from the sources and headers. First argument is the name of the library (used in src/CMakeLists.txt).
add_library(ClusterConnection STATIC ${Sources} ${Headers})
