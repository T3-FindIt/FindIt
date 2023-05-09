cmake_minimum_required(VERSION ${CMakeMinimumVersion})

set(Headers
    IClusterConnection.hpp
    # TCPClusterConnection.hpp
    TCPConnection.hpp
    Exceptions/FailedSocketInit.hpp
    Exceptions/FailedWSAStartup.hpp
)

set(Sources
    # TCPClusterConnection.cpp
    TCPConnection.cpp
)

set(GlobalIncludeDirs
    ${GlobalIncludeDirs}
    ${CMAKE_CURRENT_LIST_DIR}
    PARENT_SCOPE
)

add_library(ClusterConnection STATIC ${Sources} ${Headers})

if(WIN32)
    target_link_libraries(ClusterConnection PUBLIC wsock32 ws2_32)
endif()
