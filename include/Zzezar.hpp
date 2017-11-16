#ifndef ZZEZAR_HPP
#define ZZEZAR_HPP

#include"cipher.hpp"
#include"data_alpha.hpp"
#include <sstream>


class Zzezar : public cipher {

public:

	Zzezar(char letter_first = 'a');

	void encrypt(std::string key);

	void decrypt(std::string key);

	void crack();

private:

	float * frequency_table_;
	char * aplphabet_;
	std::size_t alphabet_len_;
	int key_;
	char letter_first_;
	
	bool prepare_to_modify(std::string & key);

	bool from_this_alphabet(char letter);

	void encr();

	void decr();

};

#endif // !ZZEZAR_HPP