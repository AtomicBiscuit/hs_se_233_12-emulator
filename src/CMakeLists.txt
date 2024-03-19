add_executable(Main main.cpp)

add_library(Parser parser.cpp)

add_library(Commands commands.cpp)

add_library(Preprocessor INTERFACE prep.h)

add_library(Emulator INTERFACE cpu.h)

add_library(DataTypes INTERFACE data.h)

target_include_directories(Commands PUBLIC
        "${PROJECT_SOURCE_DIR}/lib/stack/src"
        "${PROJECT_SOURCE_DIR}/exceptions"
)

target_link_libraries(Commands PUBLIC Stack DataTypes Exceptions)

target_link_libraries(Parser PUBLIC Commands)

target_link_libraries(Preprocessor INTERFACE Parser)

target_link_libraries(Emulator INTERFACE Preprocessor)

target_link_libraries(Main PUBLIC Emulator)

add_custom_target(Fibonacci Main run ./../../test/data/fibonacci.txt.emu DEPENDS ./../../test/data/fibonacci.txt.emu)

add_custom_command(
        OUTPUT ./../../test/data/fibonacci.txt.emu
        COMMAND Main build ./../../test/data/fibonacci.txt
)

add_custom_target(Factorial Main run ./../../test/data/factor_rec.txt.emu DEPENDS ./../../test/data/factor_rec.txt.emu)

add_custom_command(
        OUTPUT ./../../test/data/factor_rec.txt.emu
        COMMAND Main build ./../../test/data/factor_rec.txt
)

add_custom_target(B_square_minus_4AC Main run ./../../test/data/test.txt.emu DEPENDS ./../../test/data/test.txt.emu)

add_custom_command(
        OUTPUT ./../../test/data/test.txt.emu
        COMMAND Main build ./../../test/data/test.txt
)
