cmake_minimum_required(VERSION ${CMakeMinimumVersion})

set(Headers
    IDatabase.hpp
    PlainFileDatabase.hpp
)

set(Sources
    PlainFileDatabase.cpp
)

set(GlobalIncludeDirs
    ${GlobalIncludeDirs}
    ${CMAKE_CURRENT_LIST_DIR}
    PARENT_SCOPE
)

add_library(Database STATIC ${Sources} ${Headers})
