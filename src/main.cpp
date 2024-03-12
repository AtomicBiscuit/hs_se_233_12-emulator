#include <iostream>
#include "stack.h"
#include "commands.h"
#include "parser.h"
#include "exc.h"

using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        return 0;
    }
    cout << argv[1] << endl;
    try {
        Parser parser(argv[1]);
        parser.parse();
        std::vector<std::tuple<BaseCommand *, std::string>> program = parser.get_program();
        shared_ptr<CommandStack> stack(new CommandStack());
        int line = 0;
        for (auto [command, param]: program) {
            try {
                command->configure(param, line, stack);
            } catch (InvalidArgumentException &e) {
                throw InvalidArgumentException(e.what(), line + 1);
            }
            line++;
        }
        line = Begin::instance().get_line();
        while (-1 < line && line < program.size()) {
            auto [command, param] = program[line];
            try {
                line = command->run(param, line);
            } catch (InvalidArgumentException &e) {
                throw InvalidArgumentException(e.what(), line + 1);
            } catch (std::runtime_error &e) {
                throw InvalidArgumentException(e.what(), line + 1);
            }
        }
    } catch (InvalidArgumentException &e) {
        cout << "Error in line " << e.line() << ": " << e.what() << endl;
    } catch (UniqueException &e) {
        cout << "Error in line " << e.line() << ": " << e.what() << endl;
    } catch (runtime_error &e) {
        cout << "Error: " << e.what() << endl;
    }
    return 0;
}