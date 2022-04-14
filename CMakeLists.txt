cmake_minimum_required(VERSION 3.0.0)
project(Netocracy VERSION 0.1.0)

include(CTest)
include(GoogleTest)
enable_testing()

add_executable(Netocracy main.cpp)
add_executable(NetocracyTest test.cpp server/test/mapTest.cpp server/map.cpp)
target_include_directories(Netocracy PUBLIC
                           "server/include"
                           )
target_include_directories(NetocracyTest PUBLIC
"server/include"
)
target_link_libraries(NetocracyTest gtest)
target_link_libraries(NetocracyTest pthread)
gtest_discover_tests(NetocracyTest)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
