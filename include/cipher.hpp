#ifndef CIPHER_HPP
#define CIPHER_HPP

#include<iostream>
#include<fstream>
#include<string>


class cipher {

public:

	cipher();

	void text_source_in(std::istream & in);

	void text_modified_out(std::ostream & out);

	void text_source_in(std::ifstream & in);

	void text_modified_out(std::ofstream & out);

	void set_text_source(std::string const & text_source);

	std::string  get_text_source() const;

	std::string  get_text_modified() const;

	virtual void encrypt(std::string key) = 0;

	virtual void decrypt(std::string key) = 0;

	virtual void crack() = 0;

	bool is_modified();

protected:

	bool modified_; // to set true if encrypt, decrypt, crack called
	std::string text_source_;
	std::string text_modified_;

};

#endif // !CIPHER_HPP