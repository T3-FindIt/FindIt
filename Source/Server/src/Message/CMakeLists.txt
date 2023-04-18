include(ClientToServerMessages/CMakeLists.txt)
include(ServerToClientMessages/CMakeLists.txt)
include(MessageQueue/CMakeLists.txt)

list(APPEND Headers
    ${CMAKE_CURRENT_LIST_DIR}/IMessage.hpp
)
