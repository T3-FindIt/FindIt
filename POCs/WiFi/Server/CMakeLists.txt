set(CMakeMinimumVersion 3.22.1)

cmake_minimum_required(VERSION ${CMakeMinimumVersion})

set(ProjectName FindItServer)
set(ProjectRunName ${ProjectName}_Run)
set(ProjectTestName ${ProjectName}_Test)

project(${ProjectName} C CXX) # project uses C and C++ languages

set(CMAKE_C_STANDARD 99) # C standard is C99
set(CMAKE_CXX_STANDARD 17) # C++ standard is C++17
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options(-Wall -Wextra -Wpedantic)
    # add_compile_options(-Werror)
endif(CMAKE_COMPILER_IS_GNUCXX)

enable_testing()

# get google test
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.12.1
)
FetchContent_MakeAvailable(googletest)
# get google test

add_subdirectory(src)
# add_subdirectory(test)
