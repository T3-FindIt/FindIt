set(Sources
ItemTypeTest.cpp
)

set(TestName ${ProjectTestName}_ItemType)

add_executable(${TestName} ${Sources})

target_link_libraries(${TestName} PUBLIC
    gtest_main
    ItemType
)

add_test(
    NAME ${TestName}
    COMMAND ${TestName}
)
