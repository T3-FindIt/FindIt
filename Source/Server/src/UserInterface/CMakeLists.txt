cmake_minimum_required(VERSION ${CMakeMinimumVersion})

set(Headers
    UserInterface.hpp
)

set(Sources
    UserInterface.cpp
)

set(GlobalIncludeDirs
    ${GlobalIncludeDirs}
    ${CMAKE_CURRENT_LIST_DIR}
    PARENT_SCOPE
)

add_library(UserInterface STATIC ${Sources} ${Headers})
