#include"Vigener.hpp"

Vigener::Vigener(char letter_first)
	:
	Cipher(),
	key_(""),
	letter_first_(letter_first),
	key_len_(0),
	frequency_table_(nullptr),
	alphabet_(nullptr) {

	log("Debug. Vigener::Creating Vigener");
	log("Info.  Vigener::First letter of alphabet: ");
	log(std::to_string(letter_first));
	switch (letter_first_) {
	case'a':
		alphabet_len_ = ENGLISH_ALPHABET_LEN;
		frequency_table_ = ENGLISH_LETTER_FREQUENCIES;
		alphabet_ = ENGLISH_ALPHABET;
		break;
	default:
		log("Fatall. Vigener::has no this alphabet");
		throw std::invalid_argument("Invalid alphabet");
		break;
	}
	log("Debug. Vigener::Created");

}

void Vigener::encrypt(const std::string& key) {

	log("Debug. Vigener::want to encrypt");
	if (!prepare_to_modify(key)) {
		throw std::invalid_argument("Invalid key");
	}
	encr();
}

void Vigener::decrypt(const std::string& key) {

	log("Debug. Vigener::want to decrypt");
	if (!prepare_to_modify(key)) {
		throw std::invalid_argument("Invalid key");
	}
	decr();
}

void Vigener::crack() {

	log("Debug. Vigener::Trying to crack");
	text_to_lower();
	if (text_source_.length() == 0) {
		log("Warn. Vigener::text is empty to crack");
		log("Debug. Vigener::refused to crack");
		return;
	}
	key_len_ = 0;
	key_ = "";
	text_modified_ = "";

	key_len_ = find_key_len();
	text_modified_ = find_text();
	

	log("Debug. Vigener::cracking is completed");
}

void Vigener::text_to_lower() {

	log("Debug. Vigener::doing text comfortable to work with");
	std::string new_text_source = "";
	std::size_t text_source_size = text_source_.length();
	for (std::size_t i = 0; i < text_source_size; ++i) {
		if (from_this_alphabet(tolower(text_source_[i]))) {
			new_text_source += tolower(text_source_[i]);
		} else {
			log("Warn.  Vigener::letter is not from selected alphabet");
			log("Info.  Vigener::letter is ");
			log(std::to_string(text_source_[i]));
		}
	}

	text_source_ = new_text_source;
	log("Debug. Vigener::done text comfortable");

}

bool Vigener::prepare_to_modify(const std::string & key) {

	log("Debug. Vigener::preparing to modify");
	key_ = "";
	key_len_ = 0;

	std::size_t key_len = key.length();
	if (key_len == 0) {
		log("Error. Vigener::bad key");
		log("Info.  Vigener::key is");
		log(key);
		return false;
	}

	std::string new_key = "";
	for (std::size_t i = 0; i < key_len; ++i) {
		if (from_this_alphabet(tolower(key[i]))) {
			new_key += tolower(key[i]);
		} else {
			log("Error. Vigener::bad key");
			log("Info.  Vigener::key is");
			log(key);
			return false;
		}
	}

	text_to_lower();

	key_len_ = key_len;
	key_ = new_key;
	text_modified_ = "";
	log("Debug. Vigener::is ready to be modified");

	return true;
}

bool Vigener::from_this_alphabet(char letter) const {

	for (std::size_t i = 0; i < alphabet_len_; ++i) {
		if (letter == alphabet_[i]) {
			return true;
		}
	}

	return false;
}

void Vigener::encr() {

	log("Debug. Vigener::tring to encrypt");
	std::size_t text_source_size = text_source_.length();
	for (std::size_t i = 0; i < text_source_size; ++i) {
		char letter_modified = (text_source_[i] - letter_first_ + (key_[i % key_len_] - letter_first_)) % (alphabet_len_)+letter_first_;
		text_modified_ += letter_modified;
	}
	log("Debug. Vigener::encrypting done");
}

void Vigener::decr() {

	log("Debug. Vigener::tring to decrypt");
	std::size_t text_source_size = text_source_.length();
	for (std::size_t i = 0; i < text_source_size; ++i) {
		int pos = (text_source_[i] - letter_first_ - (key_[i % key_len_] - letter_first_));
		while (pos < 0) {
			pos += alphabet_len_;
		}
		char letter_modified = pos % (alphabet_len_) + letter_first_;
		text_modified_ += letter_modified;
	}
	log("Debug. Vigener::decrypting done");
}

std::size_t find_key_len() {

	std::size_t text_len = text_source_.length();
	std::size_t prob_key_len = 0;
	std::size_t cur_key_len = 1;
	std::size_t max_prob_key_len = text_len / 2;
	double max_match_index = 0;

	std::size_t * letters_count = new std::size_t[alphabet_len_];
	while (cur_key_len <= max_prob_key_len) {
		std::size_t various_letter_count = 0;
		double cur_match_index = 0;
		for (std::size_t i = 0; i < alphabet_len_; ++i) {
			letters_count[i] = 0;
		}

		for (std::size_t i = cur_key_len - 1; i < text_len; i += cur_key_len) {
			if (letters_count[text_source[i] - letter_first_] == 0) {
				++various_letter_count;
			}
			letters_count[text_source[i] - letter_first_] += 1;
		}

		if (!(various_letter_count > 1)) {
			continue;
		}

		for (std::size_t i = 0; i < alphabet_len_; ++i) {
			if (letters_count[i] > 1) {
				cur_match_index += letters_count[i] * (letters_count[i] - 1) / (various_letter_count * (various_letter_count - 1));
			}
		}

		if (cur_match_index > max_match_index) {
			max_match_index = cur_match_index;
			max_prob_key_len = cur_key_len;
		}

		++cur_key_len;
		//divide text on parts
		//parts with every cur_key_len of this text
		//various letters of this text_parts, count all and count of every
		//sum indexes
		//again
	}
	delete[] letters_count;
	return max_prob_key_len;
	
}

std::string find_text() {

	//known key_len
	//divide text on parts
	//this parts help to find letters of key with Caesar crack
	//or can use alphabet_len^key_len

	return "";
}

Vigener::~Vigener() {

}