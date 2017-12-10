#include "Console_manager.hpp"
#include <boost/algorithm/string.hpp>
#include "Caesar.hpp"
#include "Affine.hpp"
#include "SimpleSubstitution.hpp"
#include "Polybius.hpp"
#include "Vigener.hpp"

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

enum class Action
{
    Encrypt, Decrypt, Crack
};

Console_manager::Console_manager() : cipher_(nullptr), var_map_()
{}

Console_manager::~Console_manager()
{
    delete cipher_;
}

void Console_manager::parse_command_args(int argc, char** argv)
{
    po::options_description desc("General options");
    desc.add_options()
            ("help,h", "Show help")
            ("encrypt,e", "Encrypt")
            ("decrypt,d", "Decrypt")
            ("crack,c", "Crack")
            ;
    po::options_description ciphers_desc("Cipher options");
    ciphers_desc.add_options()
            ("Caesar,C", "Caesar cipher")
            ("Affine,A", "Affine cipher")
            ("Vigenere,V", "Vigenere cipher")
            ("SimpleSub,S", "Simple substitution cipher")
            ("Polybius,P", "Polybius cipher")
            ;
    po::options_description encrypt_desc("Encrypt options");
    encrypt_desc.add_options()
            ("input,i", po::value<std::string>(), "Input file")
            ("key,k", po::value<std::string>(), "Key")
            ("output,o", po::value<std::string>(), "Output file")
            ;
    po::options_description decrypt_desc("Decrypt options");
    decrypt_desc.add_options()
            ("input,i", po::value<std::string>(), "Input file")
            ("key,k", po::value<std::string>(), "Key")
            ("output,o", po::value<std::string>(), "Output file")
            ;
    po::options_description crack_desc("Crack options");
    crack_desc.add_options()
            ("input,i", po::value<std::string>(), "Input file")
            ("output,o", po::value<std::string>(), "Output file")
            ;

    desc.add(ciphers_desc);
    po::parsed_options parsed = po::command_line_parser(argc, argv).options(desc).allow_unregistered().run();
    po::store(parsed, var_map_);
    po::notify(var_map_);

    if (var_map_.count("help"))
    {
        desc.add(encrypt_desc).add(decrypt_desc).add(crack_desc);
        std::cout << desc << std::endl;
        return;
    }

    if (!initialize_cipher())
    {
        std::cout << "\nUsage: crack [general options] [cipher options] [encrypt/decrypt/crack options]\n"
                "Run \"crack -h\" for more detailed information.\n\n";
        return;
    }

    if (var_map_.count("encrypt"))
    {
        desc.add(encrypt_desc);
        po::store(po::parse_command_line(argc, argv, desc), var_map_);
        do_action(Action::Encrypt);
    }
    else if(var_map_.count("decrypt"))
    {
        desc.add(decrypt_desc);
        po::store(po::parse_command_line(argc, argv, desc), var_map_);
        do_action(Action::Decrypt);
    }
    else if(var_map_.count("crack"))
    {
        desc.add(crack_desc);
        po::store(po::parse_command_line(argc, argv, desc), var_map_);
        do_action(Action::Crack);
    }
    else
    {
        std::cout << "\nUsage: crack [general options] [cipher options] [encrypt/decrypt/crack options]\n"
                "Run \"crack -h\" for more detailed information.\n\n";
    }
}

bool Console_manager::initialize_cipher()
{
    if (var_map_.count("Caesar"))
    {
        cipher_ = new Caesar;
        return true;
    }
    else if (var_map_.count("Affine"))
    {
        cipher_ = new Affine;
        return true;
    }
    else if (var_map_.count("Vigenere"))
    {
        cipher_ = new Vigener;
        return true;
    }
    else if (var_map_.count("SimpleSub"))
    {
        cipher_ = new SimpleSubstitution;
        return true;
    }
    else if (var_map_.count("Polybius"))
    {
        cipher_ = new Polybius;
        return true;
    }

    return false;
}

void Console_manager::do_action(Action action)
{
    std::string key; // a key for encrypting (decrypting)
    std::string output_path; // output path to a file with encrypted (decrypted) text

    if (var_map_.count("input"))
    {
        std::ifstream ifs(var_map_["input"].as<std::string>());
        if (!ifs)
        {
            std::cout << "\nError trying open input file\n\n";
            ifs.close();
            return;
        }
        cipher_->text_source_in(ifs);
        ifs.close();
    }
    else cipher_->text_source_in(std::cin);

    if(var_map_.count("key"))
        key = var_map_["key"].as<std::string>();

    switch (action)
    {
        case Action::Encrypt: cipher_->encrypt(key); break;
        case Action::Decrypt: cipher_->decrypt(key); break;
        case Action::Crack: cipher_->crack(); break;
    }

    if(var_map_.count("output"))
    {
        std::ofstream ofs(var_map_["output"].as<std::string>());
        if (!ofs)
        {
            std::cout << "\nError trying open output file\n\n";
            ofs.close();
            return;
        }
        cipher_->text_modified_out(ofs);
        ofs.close();
    }
    else
    {
        cipher_->text_modified_out(std::cout);
        std::cout << std::endl;
    }
}