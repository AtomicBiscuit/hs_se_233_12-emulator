add_executable(Test test.cpp)

target_link_libraries(Test PRIVATE gtest_main Stack)

target_include_directories(Test PRIVATE
        "${PROJECT_SOURCE_DIR}/lib/stack/src"
)