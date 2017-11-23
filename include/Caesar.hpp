#ifndef ZZEZAR_HPP
#define ZZEZAR_HPP

#include"Cipher.hpp"
#include"data_alphabet.hpp"
#include <sstream>


class Caesar : public Cipher {

public:

	Caesar(char letter_first = 'a');

	void encrypt(const std::string& key);

	void decrypt(const std::string& key);

	void crack();

private:

	float * frequency_table_;
	char * alphabet_;
	std::size_t alphabet_len_;
	int key_;
	char letter_first_;
	
	bool prepare_to_modify(const std::string & key);

	bool from_this_alphabet(char letter);

	void encr();

	void decr();

};

#endif // !ZZEZAR_HPP