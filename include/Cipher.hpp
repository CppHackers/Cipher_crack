#ifndef CIPHER_HPP
#define CIPHER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Log.hpp"

class Cipher {

public:

	Cipher(unsigned log_level = Log::Debug);

	void text_source_in(std::istream & in);

	void text_modified_out(std::ostream & out) const;

	void text_source_in(std::ifstream & in);

	void text_modified_out(std::ofstream & out) const;

	void set_text_source(std::string const & text_source);

	std::string  get_text_source() const;

	std::string  get_text_modified() const;

	virtual void encrypt(const std::string& key) = 0;

	virtual void decrypt(const std::string& key) = 0;

	virtual void crack() = 0;

    bool is_modified() const;

	virtual ~Cipher();

protected:

	std::string text_source_;
	std::string text_modified_;
};


template <class KeyType>
struct decryptod {

	decryptod()
		: text_("")
		, key_(KeyType())
		, factor_(0.0)
	{};
	decryptod(std::string text, KeyType key, double factor)
		: text_(text)
		, key_(key)
		, factor_(factor)
	{};

	std::string text_;
	KeyType key_;
	double factor_;
};

struct decryptod_Vigener_mod {

	decryptod_Vigener_mod(std::size_t key_len = 0, double match_index = 0.0, std::size_t top_key = 0)
		: key_len_(key_len)
		, match_index_(match_index){

		top_key_vector_.assign(top_key, decryptod<std::string>());
	}

	std::size_t key_len_;
	std::vector<decryptod<std::string>> top_key_vector_;
	double match_index_;
};

#endif // !CIPHER_HPP