cmake_minimum_required(VERSION 3.14)
project(nvidia_ci_demo)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_library(math_ops src/math_ops.cpp)

add_executable(test_math_ops tests/test_math_ops.cpp)
target_link_libraries(test_math_ops math_ops)
