cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)

include("${CMAKE_CURRENT_SOURCE_DIR}/tools/gate/cmake/HunterGate.cmake")

HunterGate(
    URL "https://github.com/cpp-pm/hunter/archive/v0.23.254.tar.gz"
    SHA1 "017e688b9848c1f695303e8cfe9f35d40e154112"
)

project(GradientBoosting)

hunter_add_package(GTest)
find_package(GTest CONFIG REQUIRED)

add_library(decision_tree STATIC ${CMAKE_CURRENT_SOURCE_DIR}/sources/decision_tree.cpp)
target_include_directories(decision_tree PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)

add_executable(tests ${CMAKE_CURRENT_SOURCE_DIR}/tests/test.cpp)
target_link_libraries(tests GTest::gtest_main decision_tree)

enable_testing()
add_test(NAME tests COMMAND tests)
