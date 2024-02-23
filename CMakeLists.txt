cmake_minimum_required(VERSION 3.10)

project(Emulator VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# set(CMAKE_CXX_FLAGS --coverage)

add_subdirectory("lib")
add_subdirectory("src")