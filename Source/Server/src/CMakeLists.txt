cmake_minimum_required(VERSION ${CMakeMinimumVersion})

# Builds a static library for gtest
# add_library(${ProjectName} STATIC main.cpp ${Sources} ${Headers}) 
add_library(${ProjectName} INTERFACE)

add_executable(${ProjectRunName} main.cpp)

# All the libs need to be linked here to make them available to the executable
target_link_libraries(${ProjectRunName} PRIVATE
Object
Database
UserInterface
MessageInterface
MessageQueue
Communication
ProtocolParser
ClusterConnection
)

if(WIN32)
    target_link_libraries(${ProjectRunName} PRIVATE wsock32 ws2_32)
endif()


add_subdirectory(Object)
add_subdirectory(Database)
add_subdirectory(UserInterface)
add_subdirectory(Message)
add_subdirectory(Communication)
