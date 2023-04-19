include(${CMAKE_CURRENT_LIST_DIR}/ClientToServerMessages/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/ServerToClientMessages/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/MessageQueue/CMakeLists.txt)

list(APPEND Headers
    ${CMAKE_CURRENT_LIST_DIR}/IMessage.hpp
)
