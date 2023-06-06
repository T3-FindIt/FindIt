cmake_minimum_required(VERSION ${CMakeMinimumVersion})

set(Headers
    ItemType.hpp
)

set(Sources
    ItemType.cpp
)

add_library(ItemType STATIC ${Sources} ${Headers})

target_include_directories(ItemType PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
