set(Sources
    ObjectTest.cpp
)

set(TestName ${ProjectTestName}_Object)

add_executable(${TestName} ${Sources})

target_link_libraries(${TestName} PUBLIC
    gtest_main
    Object
)

add_test(
    NAME ${TestName}
    COMMAND ${TestName}
)
