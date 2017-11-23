//example1

#include"Caesar.hpp"
#include<iostream>

int main() {

	Caesar z1;
	z1.set_text_source("LoGin, AdmiN! password_ !Is! password...");
	z1.encrypt("5");
	z1.text_modified_out(std::cout);
	z1.set_text_source(z1.get_text_modified());
	std::cout << '\n';
	z1.crack();
	z1.text_modified_out(std::cout);
	std::cout << "\n\n";

	Caesar z2;
	std::cout << "Print your text:\n";
	z2.text_source_in(std::cin);
	std::cout << "\nYour text_source\n: " << z2.get_text_source() << "\n";
	z2.encrypt(std::to_string(128));
	std::cout << "\nYour encrypt_text\n: " << z2.get_text_modified() << "\n";
	z2.crack();
	std::cout << "\nYour crack_text\n:" << z2.get_text_modified();

	getchar();
	return 0;
}

//helloiamthesuperman
//wearegoodatprogramming
//iamhackerhelloworld
//loginadminpasswordpassword