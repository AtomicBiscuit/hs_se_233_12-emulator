add_executable(Main main.cpp)

target_include_directories(Main PRIVATE
        "${PROJECT_SOURCE_DIR}/lib/stack"
)

target_link_libraries(Main PRIVATE Stack)
