#ifndef VIGENER_HPP
#define VEGENER_HPP

#include <sstream>
#include <unordered_set>
#include <vector>
#include "data_alphabet.hpp"
#include "Cipher.hpp"
#include "Caesar.hpp"

#define MAX_KEY_LEN 15
#define TOP_KEY_LEN 2
#define TOP_KEY 2

class Vigener : public Cipher {

public:

	Vigener(char letter_first = 'a', std::size_t max_key_len = MAX_KEY_LEN,
		bool need_spaces = false, unsigned log_level = Log::Debug,
		unsigned top_key_len = TOP_KEY_LEN, unsigned top_key = TOP_KEY);
	void encrypt(const std::string & key) override;
	void decrypt(const std::string & key) override;
	void crack() override;
	std::string get_key() const;
	void change_spaces_mode();
	std::ostream & top_out(std::ostream & out);
	std::ofstream & top_out(std::ofstream & out);
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
	std::vector<decryptod_Vigener_mod> top_vector_;
	std::size_t const top_key_len_; // if 0 then don't waste the time, it's not more then max_key_len_
	std::size_t const top_key_; // it's not more then pow(top_key_len_, aplphabet_len_)
	std::size_t * letters_count_;

	bool prepare_to_modify(const std::string & key);
	bool from_this_alphabet(char letter) const;
	void encr();
	void decr();
	void text_to_lower();
	std::size_t find_key_len();
	std::string find_text();
	std::string find_top_text();
	std::string spaces_reborn(std::string const & text_modified);
	void add_decryptod_Vigener_mod(decryptod_Vigener_mod decr);
	void find_optimal_top(std::vector<std::size_t> & indexes, std::vector<std::vector<decryptod<int>>> & source_variant,
		std::vector<char> & letters_source, std::size_t text_len, std::string *& text_parts,
		std::size_t top_key, std::size_t top_key_len, std::size_t cur_key_len,
		decryptod_Vigener_mod & cur_decr, std::size_t deep);
	char find_key_letter(char letter_source, char letter_modified);
	void add_decryptod(decryptod_Vigener_mod & decr_V_m, decryptod<std::string> decr_C);
	double calc_factor(std::string const & text);
};

#endif // !VIGENER_HPP

