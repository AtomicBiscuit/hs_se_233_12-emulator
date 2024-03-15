add_executable(Test test.cpp)

target_link_libraries(Test PRIVATE gtest_main Commands Parser)

target_include_directories(Test PRIVATE
        "${PROJECT_SOURCE_DIR}/src"
)