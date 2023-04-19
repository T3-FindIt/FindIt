include(${CMAKE_CURRENT_LIST_DIR}/ClusterConnection/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/ProtocolParser/CMakeLists.txt)

list(APPEND Sources
    ${CMAKE_CURRENT_LIST_DIR}/Communication.cpp
)
list(APPEND Headers
    ${CMAKE_CURRENT_LIST_DIR}/Communication.hpp
)
