cmake_minimum_required(VERSION ${CMakeMinimumVersion})

set(Headers
    Object.hpp
)

set(Sources
    Object.cpp
)

add_library(Object STATIC ${Sources} ${Headers})
target_include_directories(Object PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
