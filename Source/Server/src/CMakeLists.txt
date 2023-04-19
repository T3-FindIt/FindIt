cmake_minimum_required(VERSION ${CMakeMinimumVersion})

set(Headers
)

set(Sources
)

include(UserInterface/CMakeLists.txt)
include(Object/CMakeLists.txt)
include(Message/CMakeLists.txt)
include(Database/CMakeLists.txt)
include(Communication/CMakeLists.txt)

# Builds a static library for gtest
add_library(${ProjectName} STATIC ${Sources} ${Headers})

# Builds an executable to run
add_executable(${ProjectRunName} main.cpp ${Sources} ${Headers})

if(WIN32)
    target_link_libraries(${ProjectRunName} wsock32 ws2_32 nlohmann_json::nlohmann_json)
endif()

# target_link_libraries(${ProjectRunName} PUBLIC
#     nlohmann_json::nlohmann_json
#     ${ProjectName}
# )

# add_subdirectory(Example)

# target_link_libraries(${ProjectName} Server)
# target_link_libraries(${ProjectRunName} Server)
