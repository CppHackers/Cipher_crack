#include"Caesar.hpp"
#include"Vigener.hpp"
#include<iostream>
#include<fstream>

void example1();
void example2();
void example3();

int main() {
	
	Vigener v1;
	std::ifstream in("file1.txt");
	v1.text_source_in(in);
	std::cout << "Source:\n" << v1.get_text_source() << "\n";
	v1.encrypt("milk");
	std::cout << v1.get_text_modified();
	v1.set_text_source(v1.get_text_modified());
	v1.crack();
	std::cout << "\nCRACKED:\n" << v1.get_text_modified();
	in.close();

	std::ofstream out("file_out.txt");
	out << "SOURCE:\n" << v1.get_text_source() << "\n";
	out << "KEY:\n" << v1.get_key() << "\n";
	out << "CRACK:\n";
	v1.text_modified_out(out);
	out.close();
	
	getchar();
	getchar();
	return 0;
}

void example1() {
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
}

void example2() {
	Caesar z1;
	int key = 1;
	std::string text = "Populatioan of this planet";
	z1.set_text_source(text);
	z1.encrypt(std::to_string(key));
	std::cout << "Source:\n" << text << "\n\n";

	std::string blabla = z1.get_text_modified();
	z1.set_text_source(blabla);
	std::cout << "Blablabla:\n" << blabla << "\n\n";

	z1.crack();
	text = z1.get_text_modified();
	std::cout << "Cracked:\n" << text << "\n\n";
}

void example3() {

	Vigener v1;
	v1.set_text_source("ATTACKATDAWN");
	v1.encrypt("LEMON");
	std::cout << v1.get_text_modified();
	v1.set_text_source(v1.get_text_modified());
	v1.decrypt("LEMON");
	std::cout << "\n" << v1.get_text_modified();
}

///////////////////////good
//Populatioan of this planet
//helloiamthesuperman
//wearegoodatprogramming
//iamhackerhelloworld
//loginadminpasswordpassword