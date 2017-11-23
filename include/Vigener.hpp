#ifndef VIGENER_HPP
#define VEGENER_HPP

#include"Cipher.hpp"
#include"data_alphabet.hpp"
#include <sstream>


class Vigener : public Cipher {

public:

	Vigener(char letter_first = 'a');

	void encrypt(const std::string & key) override;

	void decrypt(const std::string & key) override;

	void crack() override;

	~Vigener();

private:

	std::string key_;
	float * frequency_table_;
	char * alphabet_;
	std::size_t alphabet_len_;
	std::size_t key_len_;
	char letter_first_;

	bool prepare_to_modify(const std::string & key);

	bool from_this_alphabet(char letter) const;

	void encr();

	void decr();

	void text_to_lower();

};

#endif // !VIGENER_HPP

