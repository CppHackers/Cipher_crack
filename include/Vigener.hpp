#ifndef VIGENER_HPP
#define VEGENER_HPP

#include"Cipher.hpp"
#include"data_alphabet.hpp"
#include <sstream>
#include <unordered_set>

#define MAX_KEY_LEN 15

class Vigener : public Cipher {

public:

	Vigener(char letter_first = 'a', std::size_t max_key_len = MAX_KEY_LEN, bool need_spaces = true);
	void encrypt(const std::string & key) override;
	void decrypt(const std::string & key) override;
	void crack() override;
	std::string get_key() const;
	void change_spaces_mod();
	~Vigener();

private:

	float * frequency_table_;
	char * alphabet_;
	std::string key_;
	double match_index_;
	std::size_t alphabet_len_;
	std::size_t key_len_;
	std::size_t const max_key_len_;
	char letter_first_;
	std::unordered_set<std::size_t> spaces_pos_;
	bool need_spaces_;

	bool prepare_to_modify(const std::string & key);
	bool from_this_alphabet(char letter) const;
	void encr();
	void decr();
	void text_to_lower();
	std::size_t find_key_len();
	std::string find_text();
	void spaces_reborn();
	char find_key_letter(char letter_source, char letter_modified);

};

#endif // !VIGENER_HPP

