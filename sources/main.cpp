#include <iostream>
#include <fstream>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
#include "Cipher.hpp"
#include "Caesar.hpp"
#include "Affine.hpp"
#include "SimpleSubstitution.hpp"

namespace po = boost::program_options;

void crypt(const po::variables_map& vm, Cipher*& cipher, bool encrypt);
void crack(const po::variables_map& vm, Cipher*& cipher);
bool initialize_cipher(Cipher*& cipher, const po::variables_map& vm);

int main(int argc, char* argv[])
{
    po::options_description desc("General options");
    std::string task_type;
    desc.add_options()
            ("help,h", "Show help")
            ("task,t", po::value<std::string>(&task_type), "Select task: encrypt, decrypt, crack")
            ;
    po::options_description encrypt_desc("Encrypt options");
    encrypt_desc.add_options()
            ("cipher,c", po::value<std::string>(), "Select cipher: c (Caesar), a (Affine), v (Vigener), s (Simple substitution)")
            ("input,i", po::value<std::string>(), "Input .txt file")
            ("key,k", po::value<std::string>(), "Key")
            ("output,o", po::value<std::string>(), "Output .txt file")
            ;
    po::options_description decrypt_desc("Decrypt options");
    decrypt_desc.add_options()
            ("cipher,c", po::value<std::string>(), "Select cipher: c (Caesar), a (Affine), v (Vigener), s (Simple substitution)")
            ("input,i", po::value<std::string>(), "Input .txt file")
            ("key,k", po::value<std::string>(), "Key")
            ("output,o", po::value<std::string>(), "Output .txt file")
            ;
    po::options_description crack_desc("Crack options");
    crack_desc.add_options()
            ("cipher,c", po::value<std::string>(), "Select cipher: c (Caesar), a (Affine), v (Vigener), s (Simple substitution)")
            ("input,i", po::value<std::string>(), "Input .txt file")
            ("output,o", po::value<std::string>(), "Output .txt file")
            ;
    po::variables_map vm;
    Cipher* cipher;
    bool is_correct = false;
    try
    {
        po::parsed_options parsed = po::command_line_parser(argc, argv).options(desc).allow_unregistered().run();
        po::store(parsed, vm);
        po::notify(vm);
        if (task_type == "encrypt")
        {
            desc.add(encrypt_desc);
            po::store(po::parse_command_line(argc,argv,desc), vm);
            if (!initialize_cipher(cipher, vm))
            {
                std::cout << "Wrong cipher name.\n";
                return 0;
            }
            crypt(vm, cipher, true);
        }
        else if(task_type == "decrypt")
        {
            desc.add(decrypt_desc);
            po::store(po::parse_command_line(argc,argv,desc), vm);
            if (!initialize_cipher(cipher, vm))
            {
                std::cout << "Wrong cipher name.\n";
                return 0;
            }
            crypt(vm, cipher, false);
        }
        else if(task_type == "crack")
        {
            desc.add(crack_desc);
            po::store(po::parse_command_line(argc,argv,desc), vm);
            if (!initialize_cipher(cipher, vm))
            {
                std::cout << "Wrong cipher name.\n";
                return 0;
            }
            crack(vm, cipher);
        }
        else
        {
            desc.add(encrypt_desc).add(decrypt_desc).add(crack_desc);
            std::cout << desc << std::endl;
        }
    }
    catch(std::exception& ex)
    {
        std::cout << desc << std::endl;
    }

    return 0;
}

void crypt(const po::variables_map& vm, Cipher*& cipher, bool encrypt)
{
    std::string input_path; // input path to a file with a text to encrypt (decrypt)
    std::string key; // a key for encrypting (decrypting)
    std::string output_path; // output path to a file with encrypted (decrypted) text
    if (vm.count("input"))
        input_path = vm["input"].as<std::string>();
    if(vm.count("key"))
        key = vm["key"].as<std::string>();
    if(vm.count("output"))
        output_path = vm["output"].as<std::string>();

    std::ifstream ifs(input_path);
    if (!ifs)
    {
        std::cout << "Error trying open input file\n";
        ifs.close();
        return;
    }
    cipher->text_source_in(ifs);
    ifs.close();

    try
    {
        if (encrypt) cipher->encrypt(key);
        else cipher->decrypt(key);
    }
    catch(const std::invalid_argument& ex)
    {
        std::cout << ex.what() << std::endl;
        return;
    }

    std::ofstream ofs(output_path);
    if (!ofs)
    {
        std::cout << "Error trying open output file\n";
        ofs.close();
        return;
    }
    cipher->text_modified_out(ofs);
    ofs.close();

    if (encrypt) std::cout<<"encrypt: "<<input_path<<" "<<key<<" "<<output_path<<std::endl;
    else std::cout<<"decrypt: "<<input_path<<" "<<key<<" "<<output_path<<std::endl;
}

void crack(const po::variables_map& vm, Cipher*& cipher)
{
    std::string input_path; // input path to a file with a text to crack
    std::string output_path; // output path to a file with cracked text
    if (vm.count("input"))
        input_path = vm["input"].as<std::string>();
    if(vm.count("output"))
        output_path = vm["output"].as<std::string>();

    std::ifstream ifs(input_path);
    if (!ifs)
    {
        std::cout << "Error trying open input file\n";
        ifs.close();
        return;
    }
    cipher->text_source_in(ifs);
    ifs.close();

    try
    {
        cipher->crack();
    }
    catch(const std::invalid_argument& ex)
    {
        std::cout << ex.what() << std::endl;
        return;
    }

    std::ofstream ofs(output_path);
    if (!ofs)
    {
        std::cout << "Error trying open output file\n";
        ofs.close();
        return;
    }
    cipher->text_modified_out(ofs);
    ofs.close();

    std::cout<<"crack: "<<input_path<<" "<<output_path<<std::endl;
}

bool initialize_cipher(Cipher*& cipher, const po::variables_map& vm)
{
    if (vm.count("cipher"))
    {
        std::string cipher_type = vm["cipher"].as<std::string>();
        if (cipher_type == "c" || cipher_type == "Caesar" || cipher_type == "caesar" || cipher_type == "C")
        {
            cipher = new Caesar;
            return true;
        }
        else if (cipher_type == "a" || cipher_type == "Affine" || cipher_type == "affine" || cipher_type == "A")
        {
            cipher = new Affine;
            return true;
        }
        else if (cipher_type == "v" || cipher_type == "Vigener" || cipher_type == "vigener" || cipher_type == "V")
        {
            //cipher = new Vigener;
            return true;
        }
        else if (cipher_type == "s" || cipher_type == "Simple substitution" || cipher_type == "simple substitution" || cipher_type == "S")
        {
            cipher = new SimpleSubstitution;
            return true;
        }
    }

    return false;
}