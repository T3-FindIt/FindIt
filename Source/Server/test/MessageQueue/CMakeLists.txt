set(Sources
    MessageQueue.cpp
)

set(TestName ${ProjectTestName}_MessageQueue)

add_executable(${TestName} ${Sources})

target_link_libraries(${TestName} PUBLIC
    gtest_main
    MessageQueue
    MessageInterface
)

add_test(
    NAME ${TestName}
    COMMAND ${TestName}
)
