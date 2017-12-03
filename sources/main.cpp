#include "Console_manager.hpp"

int main(int argc, char* argv[])
{
    Console_manager manager;
    try
    {
        manager.parse_command_args(argc, argv);
    }
    catch (const std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
    }

    return 0;
}