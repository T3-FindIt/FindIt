cmake_minimum_required(VERSION ${CMakeMinimumVersion})

set(Headers
    IDatabase.hpp
    PlainFileDatabase.hpp
)

set(Sources
    PlainFileDatabase.cpp
)

add_library(Database STATIC ${Sources} ${Headers})

target_link_libraries(Database PRIVATE
    ItemType
)

target_include_directories(Database PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
