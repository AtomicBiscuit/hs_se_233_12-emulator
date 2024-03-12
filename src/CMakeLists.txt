add_executable(Main main.cpp)

add_library(Parser parser.cpp)

add_library(Commands commands.cpp )

add_library(DataTypes INTERFACE data.h)

target_include_directories(Commands PUBLIC
        "${PROJECT_SOURCE_DIR}/lib/stack/src"
        "${PROJECT_SOURCE_DIR}/exceptions"
)

target_link_libraries(Commands PUBLIC Stack DataTypes Exceptions)

target_link_libraries(Parser PUBLIC Commands)

target_link_libraries(Main PUBLIC Parser)