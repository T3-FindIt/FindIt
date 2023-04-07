cmake_minimum_required(VERSION ${CMakeMinimumVersion})

set(Sources
)

add_executable(${ProjectTestName} ${Sources})

target_link_libraries(${ProjectTestName} PUBLIC
    gtest_main
    nlohmann_json::nlohmann_json
    ${ProjectName}
)

add_test(
    NAME ${ProjectTestName}
    COMMAND ${ProjectTestName}
)
