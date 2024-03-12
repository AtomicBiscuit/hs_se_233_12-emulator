add_executable(Main main.cpp)

add_library(Parser parser.cpp)

add_library(Commands commands.cpp)

add_library(DataTypes INTERFACE data.h)

target_include_directories(Commands PUBLIC
        "${PROJECT_SOURCE_DIR}/lib/stack/src"
        "${PROJECT_SOURCE_DIR}/exceptions"
)

target_link_libraries(Commands PUBLIC Stack DataTypes Exceptions)

target_link_libraries(Parser PUBLIC Commands)

target_link_libraries(Main PUBLIC Parser)


# add_executable(Fibonachi Main)

add_custom_target(Fibonachi Main "./../../test/data/fibonachi.txt")

add_custom_target(B_kvagrat_minus_4AC Main "./../../test/data/test.txt")