cmake_minimum_required(VERSION 3.10)

project(Emulator VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# set(CMAKE_CXX_FLAGS --coverage)

add_subdirectory("lib")
add_subdirectory("exceptions")
add_subdirectory("src")
add_subdirectory("test")

enable_testing()

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)