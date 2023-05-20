set(Sources
    MessageTest.cpp
)

set(TestName ${ProjectTestName}_Message)

add_executable(${TestName} ${Sources})

target_link_libraries(${TestName} PUBLIC
    gtest_main
    MessageInterface
)

add_test(
    NAME ${TestName}
    COMMAND ${TestName}
)
