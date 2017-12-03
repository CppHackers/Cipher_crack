#ifndef CONSOLE_MANAGER_HPP
#define CONSOLE_MANAGER_HPP

#include <boost/program_options.hpp>
#include "Cipher.hpp"

namespace po = boost::program_options;

enum class Action : int;

class Console_manager
{
public:
    Console_manager();
    ~Console_manager();
    void parse_command_args(int argc, char** argv);

private:
    po::variables_map var_map_;
    Cipher* cipher_;

    bool initialize_cipher();
    void do_action(Action action);
};

#endif // CONSOLE_MANAGER_HPP