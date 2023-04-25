cmake_minimum_required(VERSION ${CMakeMinimumVersion})

# Add headers here
set(Headers
    Object.hpp
)

# Add sources here
set(Sources
    Object.cpp
)

# Makes a library from the sources and headers. First argument is the name of the library (used in src/CMakeLists.txt).
add_library(Object STATIC ${Sources} ${Headers})
