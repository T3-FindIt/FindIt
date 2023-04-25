cmake_minimum_required(VERSION ${CMakeMinimumVersion})

# Add headers here
set(Headers
    IDatabase.hpp
    PlainFileDatabase.hpp
)

# Add sources here
set(Sources
    PlainFileDatabase.cpp
)

# Makes a library from the sources and headers. First argument is the name of the library (used in src/CMakeLists.txt).
add_library(Database STATIC ${Sources} ${Headers})
