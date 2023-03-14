set(ProjectTestName ${ProjectName}_Test) # <Var name> <Name of the executable>

# Pre existing CMake code

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

add_subdirectory(test) # add test subdirectory
