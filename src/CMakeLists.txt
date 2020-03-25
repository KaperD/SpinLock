add_library(
    spinlock
    spinlock.hpp
    spinlock.cpp
)

target_include_directories(spinlock PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")