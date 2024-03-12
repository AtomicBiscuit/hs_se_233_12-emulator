add_library(Stack INTERFACE src/stack.h)

add_executable(TestStack tests/Stack.cpp)

target_link_libraries(TestStack PUBLIC gtest_main Stack)

target_include_directories(TestStack PRIVATE
        "${PROJECT_SOURCE_DIR}/lib/stack/src"
)