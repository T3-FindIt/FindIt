cmake_minimum_required(VERSION ${CMakeMinimumVersion})

set(Headers
    MessageQueue.hpp
)

set(Sources
    MessageQueue.cpp
)

set(GlobalIncludeDirs
    ${GlobalIncludeDirs}
    ${CMAKE_CURRENT_LIST_DIR}
    PARENT_SCOPE
)

add_library(MessageQueue STATIC ${Sources} ${Headers})
