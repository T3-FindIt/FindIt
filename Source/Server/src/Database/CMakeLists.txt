cmake_minimum_required(VERSION ${CMakeMinimumVersion})

set(Headers
    IDatabase.hpp
    PlainFileDatabase.hpp
)

set(Sources
    PlainFileDatabase.cpp
)

add_library(Database STATIC ${Sources} ${Headers})
