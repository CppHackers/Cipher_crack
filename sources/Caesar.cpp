#include"Caesar.hpp"

Caesar::Caesar(char letter_first)
	:
	Cipher(),
	key_(0),
	letter_first_(letter_first),
	frequency_table_(nullptr),
	alphabet_(nullptr) {

	log("Debug. Caesar::Creating Caesar");
	log("Info.  Caesar::First letter of alphabet: ");
	log(std::to_string(letter_first));
	switch (letter_first_) {
	case'a':
		alphabet_len_ = ENGLISH_ALPHABET_LEN;
		frequency_table_ = ENGLISH_LETTER_FREQUENCIES;
		alphabet_ = ENGLISH_ALPHABET;
		break;
	default:
		log("Fatall. Caesar::has no this alphabet");
		throw std::invalid_argument("Invalid alphabet");
		break;
	}
	log("Debug. Caesar::Created");

}

void Caesar::encrypt(const std::string& key) {

	log("Debug. Caesar::want to encrypt");
	if (!prepare_to_modify(key)) {
		throw std::invalid_argument("Invalid key");
	}
	encr();
}

void Caesar::decrypt(const std::string& key) {

	log("Debug. Caesar::want to decrypt");
	if (!prepare_to_modify(key)) {
		throw std::invalid_argument("Invalid key");
	}
	decr();
}

void Caesar::crack() {

	log("Debug. Caesar::Trying to crack");
	float max_probability = 0;
	int cur_key = 0;
	int max_probabbility_key = 0;
	std::size_t * letters_count = new std::size_t[alphabet_len_];
	text_to_lower();

	while (cur_key < alphabet_len_) {

		key_ = cur_key;
		text_modified_ = "";
		decr();

		for (std::size_t i = 0; i < alphabet_len_; ++i) {
			letters_count[i] = 0;
		}
		std::size_t max_letters = text_modified_.length();
		for (std::size_t i = 0; i < max_letters; ++i) {
			letters_count[text_modified_[i] - letter_first_] += 1;
		}

		float cur_probability = 0;
		for (std::size_t i = 0; i < max_letters; ++i) {
			cur_probability += ((letters_count[i] * 1.0) / max_letters )  * (frequency_table_[i] / 100);
		}
		if (cur_probability > max_probability) {
			max_probability = cur_probability;
			max_probabbility_key = cur_key;
		}
		++cur_key;
	}

	key_ = max_probabbility_key;
	text_modified_ = "";
	decr();
	delete[] letters_count;
	log("Debug. Caesar::cracking is completed");
}

void Caesar::text_to_lower() {

	log("Debug. Caesar::doing text comfortable to work with");
	std::string new_text_source = "";
	for (std::size_t i = 0; i < text_source_.length(); ++i) {
		if (from_this_alphabet(tolower(text_source_[i]))) {
			new_text_source += tolower(text_source_[i]);
		} else {
			log("Warn.  Caesar::letter is not from selected alphabet");
			log("Info.  Caesar::letter is ");
			log(std::to_string(text_source_[i]));
		}
	}

	text_source_ = new_text_source;
	log("Debug. Caesar::done text comfortable");

}

bool Caesar::prepare_to_modify(const std::string & key) {

	log("Debug. Caesar::preparing to modify");
	if (text_source_.length() == 0) {
		return false;
	}
	std::istringstream sstream(key);
	int new_key = 0;
	if (!(sstream >> new_key) || (new_key < 0)) {
		log("Error. Caesar::bad key");
		log("Info.  Caesar::key is");
		log(key);
		return false;
	}

	text_to_lower();

	key_ = new_key;
	text_modified_ = "";
	log("Debug. Caesar::is ready to be modified");

	return true;
} 

bool Caesar::from_this_alphabet(char letter) const {

	for (std::size_t i = 0; i < alphabet_len_; ++i) {
		if (letter == alphabet_[i]) {
			return true;
		}
	}

	return false;
}

void Caesar::encr() {

	log("Debug. Caesar::tring to encrypt");
	std::size_t text_source_size = text_source_.length();
	for (std::size_t i = 0; i < text_source_size; ++i) {
		char letter_modified = (text_source_[i] - letter_first_ + key_) % (alphabet_len_)+letter_first_;
		text_modified_ += letter_modified;
	}
	log("Debug. Caesar::encrypting done");
}

void Caesar::decr() {

	log("Debug. Caesar::tring to decrypt");
	std::size_t text_source_size = text_source_.length();
	for (std::size_t i = 0; i < text_source_size; ++i) {
		int pos = (text_source_[i] - letter_first_ - key_);
		while (pos < 0) {
			pos += alphabet_len_;
		}
		char letter_modified = pos % (alphabet_len_)+letter_first_;
		text_modified_ += letter_modified;
	}
	log("Debug. Caesar::decrypting done");
}