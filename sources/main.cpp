#include "Console_manager.hpp"

int main(int argc, char* argv[])
{
    Console_manager manager;
    manager.parse_command_args(argc, argv);

    return 0;
}