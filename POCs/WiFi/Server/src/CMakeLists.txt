set(Headers
    Socket.hpp
)

set(Sources
    Socket.cpp
)

# Builds a static library for gtest
add_library(${ProjectName} STATIC ${Sources} ${Headers})

# Builds an executable to run
add_executable(${ProjectRunName} main.cpp ${Sources} ${Headers})

# add_subdirectory(Example)

target_link_libraries(${ProjectName} Server)
target_link_libraries(${ProjectRunName} Server)