add_executable(Main main.cpp)

add_library(Parser parser.cpp)

add_library(Commands INTERFACE commands.h)

add_library(DataTypes INTERFACE data.h)

target_include_directories(Commands INTERFACE
        "${PROJECT_SOURCE_DIR}/lib/stack/src"
)

target_link_libraries(Commands INTERFACE Stack DataTypes)

target_include_directories(Main PRIVATE
        "${PROJECT_SOURCE_DIR}/lib/stack/src"
)

target_link_libraries(Main PRIVATE Stack Parser)

target_link_libraries(Parser PRIVATE Commands)