include(ClusterConnection/CMakeLists.txt)
include(ProtocolParser/CMakeLists.txt)

list(APPEND Sources
    ${CMAKE_CURRENT_LIST_DIR}/Communication.cpp
)
list(APPEND Headers
    ${CMAKE_CURRENT_LIST_DIR}/Communication.hpp
)
