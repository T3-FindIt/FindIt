cmake_minimum_required(VERSION ${CMakeMinimumVersion})

set(Headers
)

set(Sources
)

set(GlobalIncludeDirs
    ${CMAKE_CURRENT_LIST_DIR}
)

# Builds a static library for gtest
add_library(${ProjectName} STATIC main.cpp ${Sources} ${Headers}) 

# Builds an executable to run
add_executable(${ProjectRunName} main.cpp ${Sources} ${Headers})

if(WIN32)
    target_link_libraries(${ProjectRunName} PUBLIC wsock32 ws2_32)
endif()

target_link_libraries(${ProjectRunName} PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(${ProjectName} PRIVATE nlohmann_json::nlohmann_json)

add_subdirectory(UserInterface)
target_link_libraries(${ProjectRunName} PRIVATE UserInterface)
target_link_libraries(${ProjectName} PRIVATE UserInterface)

add_subdirectory(Communication)
target_link_libraries(${ProjectRunName} PRIVATE Communication)
target_link_libraries(${ProjectName} PRIVATE Communication)

add_subdirectory(Object)
target_link_libraries(${ProjectRunName} PRIVATE Object)
target_link_libraries(${ProjectName} PRIVATE Object)

add_subdirectory(Message)
target_link_libraries(${ProjectRunName} PRIVATE Message)
target_link_libraries(${ProjectName} PRIVATE Message)

add_subdirectory(Database)
target_link_libraries(${ProjectRunName} PRIVATE Database)
target_link_libraries(${ProjectName} PRIVATE Database)

# Hacky way to get the include directories to work
target_include_directories(ClusterConnection PRIVATE ${GlobalIncludeDirs})
target_include_directories(ProtocolParser PRIVATE ${GlobalIncludeDirs})
target_include_directories(Communication PRIVATE ${GlobalIncludeDirs})
target_include_directories(Database PRIVATE ${GlobalIncludeDirs})
target_include_directories(MessageQueue PRIVATE ${GlobalIncludeDirs})
target_include_directories(Message PRIVATE ${GlobalIncludeDirs})
target_include_directories(Object PRIVATE ${GlobalIncludeDirs})
target_include_directories(UserInterface PRIVATE ${GlobalIncludeDirs})
target_include_directories(${ProjectName} PRIVATE ${GlobalIncludeDirs})
target_include_directories(${ProjectRunName} PRIVATE ${GlobalIncludeDirs})
