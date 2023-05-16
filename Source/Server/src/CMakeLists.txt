cmake_minimum_required(VERSION ${CMakeMinimumVersion})

# Builds an executable to run
add_executable(${ProjectRunName} main.cpp)

target_link_libraries(${ProjectRunName} PRIVATE
Object
Database
UserInterface
)

if(WIN32)
    target_link_libraries(${ProjectRunName} PRIVATE wsock32 ws2_32)
endif()

# target_link_libraries(${ProjectRunName} PRIVATE nlohmann_json::nlohmann_json)

add_subdirectory(Object)
add_subdirectory(Database)
add_subdirectory(UserInterface)
