cmake_minimum_required(VERSION ${CMakeMinimumVersion})

# Add headers here
set(Headers
    UserInterface.hpp
)

# Add sources here
set(Sources
    UserInterface.cpp
)

# Makes a library from the sources and headers. First argument is the name of the library (used in src/CMakeLists.txt).
add_library(UserInterface STATIC ${Sources} ${Headers})
