cmake_minimum_required(VERSION ${CMakeMinimumVersion})

set(Headers
    # IMessage
    IMessage.hpp
    # ServerToClientMessages
    ServerToClientMessages/HeartBeat.hpp
    ServerToClientMessages/NodeNotifyNewProductResponse.hpp
    ServerToClientMessages/NodeSignInResponse.hpp
    ServerToClientMessages/ServerRequestProduct.hpp
    # ClientToServerMessages
    ClientToServerMessages/HeartBeatResponse.hpp
    ClientToServerMessages/NodeEventProductFound.hpp
    ClientToServerMessages/NodeNotifyNewProduct.hpp
    ClientToServerMessages/NodeRespondToProductRequest.hpp
    ClientToServerMessages/NodeSignIn.hpp
)

set(Sources
    # ServerToClientMessages
    ServerToClientMessages/HeartBeat.cpp
    ServerToClientMessages/NodeNotifyNewProductResponse.cpp
    ServerToClientMessages/NodeSignInResponse.cpp
    ServerToClientMessages/ServerRequestProduct.cpp
    # ClientToServerMessages
    ClientToServerMessages/HeartBeatResponse.cpp
    ClientToServerMessages/NodeEventProductFound.cpp
    ClientToServerMessages/NodeNotifyNewProduct.cpp
    ClientToServerMessages/NodeRespondToProductRequest.cpp
    ClientToServerMessages/NodeSignIn.cpp
)

add_library(Message STATIC ${Sources} ${Headers})

add_subdirectory(MessageQueue)
target_link_libraries(Message PRIVATE MessageQueue)
# target_include_directories(Message PRIVATE ${CMAKE_CURRENT_LIST_DIR})
