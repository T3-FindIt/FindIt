cmake_minimum_required(VERSION ${CMakeMinimumVersion})

set(Headers
    MessageQueue.hpp
)

set(Sources
    MessageQueue.cpp
)

add_library(MessageQueue STATIC ${Sources} ${Headers})
