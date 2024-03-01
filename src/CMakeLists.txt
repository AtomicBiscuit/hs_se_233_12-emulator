add_executable(Main main.cpp)

add_library(Parser parser.cpp)

target_include_directories(Main PRIVATE
        "${PROJECT_SOURCE_DIR}/lib/stack"
)

target_link_libraries(Main PRIVATE Stack, Parser)
