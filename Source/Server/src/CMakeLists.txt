cmake_minimum_required(VERSION ${CMakeMinimumVersion})

add_executable(${ProjectRunName} main.cpp)

# All the libs need to be linked here to make them available to the executable
target_link_libraries(${ProjectRunName} PRIVATE
ItemType
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

add_subdirectory(ItemType)
add_subdirectory(Database)
add_subdirectory(UserInterface)
add_subdirectory(Message)
add_subdirectory(Communication)
