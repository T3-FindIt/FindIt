set(Sources
    JSONTest.cpp
)

set(TestName ${ProjectTestName}_JSON)

add_executable(${TestName} ${Sources})

target_link_libraries(${TestName} PUBLIC
    gtest_main
    ProtocolParser
    MessageInterface
)

add_test(
    NAME ${TestName}
    COMMAND ${TestName}
)
