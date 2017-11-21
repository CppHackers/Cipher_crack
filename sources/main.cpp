#include <limits>
#include <typeinfo>
#include "Zzezar.hpp"
#include "Affine.hpp"

void get_text_modified(cipher*& c)
{
    char choice;

    while (true)
    {
        std::cout << "\nPrint:\n"
                "\'p\' - if you want to write modified text in the standard output stream;\n"
                "\'f\' - if you want to write modified text in the file;\n"
                "\'q\' - if you want to exit: ";

        if (!(std::cin >> choice)) continue;

        choice = static_cast<char>(tolower(choice));

        if (choice == 'p')
        {
            std::cout << "\nModified text:\n";
            c->text_modified_out(std::cout);
            std::cout << std::endl;
        }
        else if (choice == 'f')
        {
            std::string name;
            std::cout << "\nFile name: ";
            std::getline(std::cin, name);
            std::ofstream ofs(name);
            if (!ofs)
            {
                std::cout << "Cannot open file.\n";
                continue;
            }
            std::cout << std::endl;
            std::cout << "\nModified text:\n";
            c->text_modified_out(ofs);
        }
        else if (choice == 'q') return;
    }
}

void modify(cipher*& c)
{
    char choice;

    while (true)
    {
        std::cout << "\nYour text source:\n";
        std::cout << c->get_text_source() << std::endl;

        std::cout << "\nPrint:\n"
                "\'e\' - if you want to encrypt text source;\n"
                "\'d\' - if you want to decrypt text source;\n"
                "\'c\' - if you want to crack text source;\n"
                "\'q\' - if you want to exit: ";

        if (!(std::cin >> choice)) continue;

        choice = static_cast<char>(tolower(choice));
        if (choice == 'e')
        {
            std::string key;

            auto a = typeid(*c).name();
            auto b = typeid(Zzezar).name();
            if (typeid(*c).name() == typeid(Zzezar).name())
                std::cout << "\nPrint key - an integer between 1 and the alphabet length.\n";
            else if (typeid(*c).name() == typeid(Affine).name())
                std::cout << "\nPrint key in format \"a,b\", where a and b - integers.\n"
                        "a and the alphabet length must me comprime integers.\n";

            std::cout << "Key: ";
            std::cin >> key;
            try
            {
                c->encrypt(key);
            }
            catch (const std::exception &ex)
            {
                std::cout << ex.what() << std::endl;
                continue;
            }
            std::cout << "\nText source has been encrypted.\n";
            get_text_modified(c);
            break;
        }
        else if (choice == 'd')
        {
            std::string key;

            if (typeid(*c).name() == typeid(Zzezar).name())
                std::cout << "\nPrint key - an integer between 1 and the alphabet length.\n";
            else if (typeid(*c).name() == typeid(Affine).name())
                std::cout << "\nPrint key in format \"a,b\", where a and b - integers.\n"
                        "a and the alphabet length must me comprime integers.\n";

            std::cout << "Key: ";
            std::cin >> key;
            try
            {
                c->decrypt(key);
            }
            catch (const std::exception &ex)
            {
                std::cout << ex.what() << std::endl;
                continue;
            }
            std::cout << "\nText source has been decrypted.\n";
            get_text_modified(c);
            break;
        }
        else if (choice == 'c')
        {
            c->crack();
            std::cout << "\nText source has been cracked.\n";
            get_text_modified(c);
            break;
        }
        else if (choice == 'q')return;
    }
}

void get_text_source(cipher*& c)
{
    char choice;
    while (true)
    {
        std::cout << "\nSet text source. Print:\n"
                "\'p\' - if you want to print it;\n"
                "\'f\' - if you want to get it from a file;\n"
                "\'m\' - if you want to set modified text as text source;\n"
                "\'c\' - if you want to continue;\n"
                "\'s\' - if you want to show text source;\n"
                "\'q\' - if you want to exit: ";

        if (!(std::cin >> choice)) continue;

        choice = static_cast<char>(tolower(choice));
        if (choice == 'p')
        {
            std::cin.ignore();
            std::cout << "Press enter twice to finish.\n";
            c->text_source_in(std::cin);
            modify(c);
        }
        else if (choice == 'f')
        {
            std::string name;
            std::cout << "\nFile name: ";
            std::getline(std::cin, name);
            std::ifstream ifs(name);
            if (!ifs)
            {
                std::cout << "Cannot open file.\n";
                continue;
            }
            c->text_source_in(ifs);
            modify(c);
        }
        else if (choice == 'm')
        {
            c->set_text_source(c->get_text_modified());
            modify(c);
        }
        else if (choice == 'c') modify(c);
        else if (choice == 's') std::cout << "Your text source:\n" << c->get_text_source() << std::endl;
        else if (choice == 'q') break;
    }
}

int main()
{
    char choice;

    while (true)
    {
        std::cout << "\nPrint:\n"
                "\'c\' - if you want to work with Caesar cipher;\n"
                "\'a\' - if you want to work with Affine cipher;\n"
                "\'q\' - if you want to exit: ";

        if (!(std::cin >> choice)) continue;

        choice = static_cast<char>(tolower(choice));
        if (choice == 'c')
        {
            cipher* z = new Zzezar();
            get_text_source(z);
        }
        else if (choice == 'a')
        {
            cipher* a = new Affine();
            get_text_source(a);
        }
        else if (choice == 'q') break;
    }

    return 0;
}