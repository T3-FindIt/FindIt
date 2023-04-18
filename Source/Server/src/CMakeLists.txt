cmake_minimum_required(VERSION ${CMakeMinimumVersion})

set(Headers
)

set(Sources
)

include(UserInterface/CMakeLists.txt)

# Builds a static library for gtest
add_library(${ProjectName} STATIC ${Sources} ${Headers})

# Builds an executable to run
add_executable(${ProjectRunName} main.cpp ${Sources} ${Headers})

if(WIN32)
    target_link_libraries(${ProjectRunName} wsock32 ws2_32)
endif()

target_link_libraries(${ProjectRunName} nlohmann_json::nlohmann_json)

# add_subdirectory(Example)

# target_link_libraries(${ProjectName} Server)
# target_link_libraries(${ProjectRunName} Server)
