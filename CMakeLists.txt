cmake_minimum_required(VERSION 3.10)

project(Emulator VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# set(CMAKE_CXX_FLAGS --coverage)

add_subdirectory("lib")
add_subdirectory("src")

add_executable(Lol src/main.cpp)

target_link_libraries(Lol PRIVATE Stacker)

target_include_directories(Lol PRIVATE
    "${PROJECT_SOURCE_DIR}/lib"
)