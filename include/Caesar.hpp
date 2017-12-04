#ifndef CAESAR_HPP
#define CAESAR_HPP

#include <sstream>
#include <vector>
#include <unordered_set>
#include"data_alphabet.hpp"
#include"Cipher.hpp"

#define TOP 3

class Caesar : public Cipher {

public:

	Caesar(char letter_first = 'a', bool need_spaces = false,
		unsigned log_level = Log::Debug, unsigned top = TOP);
	void encrypt(const std::string & key) override;
	void decrypt(const std::string & key) override;
	void crack() override;
	~Caesar();
	void change_spaces_mode();
	std::string spaces_reborn(std::string const & text_modified);
	std::vector<decryptod<int>> get_top();
	std::ostream & top_out(std::ostream & out);
	std::ofstream & top_out(std::ofstream & out);

private:

	float * frequency_table_;
	char * alphabet_;
	std::size_t alphabet_len_;
	int key_; // !!! unsigned 
	char letter_first_;
	std::vector<decryptod<int>> top_vector_;
	std::unordered_set<std::size_t> spaces_pos_;
	bool need_spaces_;
	std::size_t const top_; // if 0 then don't waste the time, it's not more then alphabet_len_
	std::size_t * letters_count_;


	bool prepare_to_modify(const std::string & key);
	bool from_this_alphabet(char letter) const;
	void encr();
	void decr();
	void text_to_lower();
	void add_decryptod(decryptod<int> decr);

};

#endif // !CAESAR_HPP