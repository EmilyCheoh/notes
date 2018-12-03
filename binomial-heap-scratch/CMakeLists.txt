cmake_minimum_required(VERSION 3.12)
project(x)

include(.ipd/cmake/CMakeLists.txt)

set(CMAKE_CXX_STANDARD 14)

add_test_program(x test.cpp)
