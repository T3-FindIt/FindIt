list(APPEND Sources
    ${CMAKE_CURRENT_LIST_DIR}/HeartbeatResponse.cpp
    ${CMAKE_CURRENT_LIST_DIR}/NodeEventProductFound.cpp
    ${CMAKE_CURRENT_LIST_DIR}/NodeNotifyNewProduct.cpp
    ${CMAKE_CURRENT_LIST_DIR}/NodeRespondToProductRequest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/NodeSignIn.cpp
)

list(APPEND Headers
    ${CMAKE_CURRENT_LIST_DIR}/HeartbeatResponse.hpp
    ${CMAKE_CURRENT_LIST_DIR}/NodeEventProductFound.hpp
    ${CMAKE_CURRENT_LIST_DIR}/NodeNotifyNewProduct.hpp
    ${CMAKE_CURRENT_LIST_DIR}/NodeRespondToProductRequest.hpp
    ${CMAKE_CURRENT_LIST_DIR}/NodeSignIn.hpp
)
