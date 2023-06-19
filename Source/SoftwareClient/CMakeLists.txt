cmake_minimum_required(VERSION 3.26.2)
project(TCP_Client VERSION 1.0.0)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(CTest)
enable_testing()

set(BUILD_SHARED_LIBS OFF)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")

add_executable(TCP_Client_exe main.cpp)

if(WIN32)
    target_link_libraries(TCP_Client_exe PUBLIC wsock32 ws2_32)
endif()

if(MSVC)
    target_compile_options(TCP_Client_exe PRIVATE /W4)
else()
    target_compile_options(TCP_Client_exe PRIVATE -Wall -Wextra -pedantic)
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
