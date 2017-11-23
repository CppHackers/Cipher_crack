#ifndef CAESAR_HPP
#define CAESAR_HPP

#include"Cipher.hpp"
#include"data_alphabet.hpp"
#include <sstream>


class Caesar : public Cipher {

public:

	Caesar(char letter_first = 'a');

	void encrypt(const std::string& key) override;

	void decrypt(const std::string& key) override;

	void crack() override;

private:

	float * frequency_table_;
	char * alphabet_;
	std::size_t alphabet_len_;
	int key_;
	char letter_first_;

	bool prepare_to_modify(const std::string & key);

	bool from_this_alphabet(char letter) const;

	void encr();

	void decr();

	void text_to_lower();

};

#endif // !CAESAR_HPP