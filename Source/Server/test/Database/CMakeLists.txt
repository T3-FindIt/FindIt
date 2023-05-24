set(Sources
    DatabaseTest.cpp
)

add_executable(${ProjectTestName}_Database ${Sources})

target_link_libraries(${ProjectTestName}_Database PUBLIC
    gtest_main
    Object
    Database
)

add_test(
    NAME ${ProjectTestName}_Database
    COMMAND ${ProjectTestName}_Database
)
