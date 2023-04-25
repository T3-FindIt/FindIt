cmake_minimum_required(VERSION ${CMakeMinimumVersion})

# Add headers here
set(Headers
    Communication.hpp
)

# Add sources here
set(Sources
    Communication.cpp
)

# Makes a library from the sources and headers. First argument is the name of the library (used in src/CMakeLists.txt).
add_library(Communication STATIC ${Sources} ${Headers})

add_subdirectory(ProtocolParser)
target_link_libraries(Communication PRIVATE ProtocolParser)

add_subdirectory(ClusterConnection)
target_link_libraries(Communication PRIVATE ClusterConnection)
