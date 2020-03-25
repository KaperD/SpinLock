cmake_minimum_required(VERSION 2.8)

project(Tests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
 
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_subdirectory(../src build)

enable_testing()

add_executable(runTests tests.cpp)
target_link_libraries(
    runTests
    ${GTEST_LIBRARIES}
    pthread
    spinlock
)

add_test(first_try runTests)