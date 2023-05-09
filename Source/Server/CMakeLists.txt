set(CMakeMinimumVersion 3.22.1)
cmake_minimum_required(VERSION ${CMakeMinimumVersion})

set(ProjectName FindItServer)
set(ProjectRunName ${ProjectName}_Run)
set(ProjectTestName ${ProjectName}_Test)

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/release )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )

project(${ProjectName} C CXX) # project uses C and C++ languages

set(CMAKE_C_STANDARD 99) # C standard is C99
set(CMAKE_CXX_STANDARD 20) # C++ standard is C++17
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(BUILD_SHARED_LIBS OFF)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")

if(CMAKE_COMPILER_IS_GNUCXX)
  add_compile_options(-Wall -Wextra -Wpedantic)
  # add_compile_options(-Werror)
endif(CMAKE_COMPILER_IS_GNUCXX)

if(MSVC)
  add_compile_options(/W4)
  # add_compile_options(/WX)
endif(MSVC)

enable_testing()

include(FetchContent)

# get google test
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.12.1
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)
# get google test

# get nlohmann json
FetchContent_Declare(
  json
  URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(json)
# get nlohmann json

add_subdirectory(src)
# add_subdirectory(test)

# Copy all used dlls into the release folder

