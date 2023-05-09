cmake_minimum_required(VERSION ${CMakeMinimumVersion})

set(Headers
    Object.hpp
)

set(Sources
    Object.cpp
)

set(GlobalIncludeDirs
    ${GlobalIncludeDirs}
    ${CMAKE_CURRENT_LIST_DIR}
    PARENT_SCOPE
)

add_library(Object STATIC ${Sources} ${Headers})
