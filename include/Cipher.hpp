#ifndef CIPHER_HPP
#define CIPHER_HPP

#include<iostream>
#include<fstream>
#include<string>
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

#endif // !CIPHER_HPP