cmake_minimum_required(VERSION ${CMakeMinimumVersion})

set(Headers
    MessageQueue.hpp
)

set(Sources
    MessageQueue.cpp
)

add_library(MessageQueue STATIC ${Sources} ${Headers})
target_include_directories(MessageQueue PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
