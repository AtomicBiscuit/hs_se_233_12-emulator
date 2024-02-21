add_executable(Mainer main.cpp)

target_include_directories(Mainer PRIVATE
        "${PROJECT_SOURCE_DIR}/lib"
)

target_link_libraries(Mainer PRIVATE Stacker)
