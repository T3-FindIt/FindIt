cmake_minimum_required(VERSION ${CMakeMinimumVersion})

set(Headers
    UserInterface.hpp
)

set(Sources
    UserInterface.cpp
)

add_library(UserInterface STATIC ${Sources} ${Headers})
target_include_directories(UserInterface PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
