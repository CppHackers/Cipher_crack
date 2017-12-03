#include"Caesar.hpp"

Caesar::Caesar(char letter_first, unsigned log_level)
	:
	Cipher(log_level),
	key_(0),
	letter_first_(letter_first),
	frequency_table_(nullptr),
	alphabet_(nullptr) {

	Log::Logger()->log(Log::Debug, "Caesar::Creating Caesar");
	Log::Logger()->log(Log::Info, " Caesar::First letter of alphabet: ");
	Log::Logger()->log(Log::Info, std::to_string(letter_first));
	switch (letter_first_) {
	case'a':
		alphabet_len_ = ENGLISH_ALPHABET_LEN;
		frequency_table_ = ENGLISH_LETTER_FREQUENCIES;
		alphabet_ = ENGLISH_ALPHABET;
		break;
	default:
		Log::Logger()->log(Log::Error, "Fatall. Caesar::has no this alphabet");
		throw std::invalid_argument("Invalid alphabet");
		break;
	}
	Log::Logger()->log(Log::Debug, "Caesar::Created");

}

void Caesar::encrypt(const std::string& key) {

	Log::Logger()->log(Log::Debug, "Caesar::want to encrypt");
	if (!prepare_to_modify(key)) {
		Log::Logger()->log(Log::Error, "Caesar::bad key");
		Log::Logger()->log(Log::Info, " Caesar::key is");
		Log::Logger()->log(Log::Info, key);
		throw std::invalid_argument("Invalid key");
	}
	encr();
}

void Caesar::decrypt(const std::string& key) {

	Log::Logger()->log(Log::Debug, "Caesar::want to decrypt");
	if (!prepare_to_modify(key)) {
		throw std::invalid_argument("Invalid key");
	}
	decr();
}

void Caesar::crack() {

	Log::Logger()->log(Log::Debug, "Caesar::Trying to crack");
	text_to_lower();

	if (text_source_.length() == 0) {
		Log::Logger()->log(Log::Warn, "Caesar::text is empty to crack");
		Log::Logger()->log(Log::Debug, "Caesar::refused to crack");
		throw(std::logic_error("Text is too short"));
		return;
	}
	float max_probability = 0;
	int cur_key = 1;
	int max_probability_key = 1;
	std::size_t * letters_count = new std::size_t[alphabet_len_];

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

		double cur_probability = 0;
		for (std::size_t i = 0; i < alphabet_len_; ++i) {
			cur_probability += ((letters_count[i] * 1.0) / max_letters )  * (frequency_table_[i] / 100);
		}
		if (cur_probability > max_probability) {
			max_probability = cur_probability;
			max_probability_key = cur_key;
		}
		++cur_key;
	}

	key_ = max_probability_key;
	text_modified_ = "";
	decr();
	delete[] letters_count;
	Log::Logger()->log(Log::Debug, "Caesar::cracking is completed");
}

void Caesar::text_to_lower() {

	Log::Logger()->log(Log::Debug, "Caesar::doing text comfortable to work with");
	std::string new_text_source = "";
	std::size_t text_source_size = text_source_.length();
	for (std::size_t i = 0; i < text_source_size; ++i) {
		if (from_this_alphabet(tolower(text_source_[i]))) {
			new_text_source += tolower(text_source_[i]);
		} else {
			Log::Logger()->log(Log::Warn, " Caesar::letter is not from selected alphabet");
			Log::Logger()->log(Log::Info, " Caesar::letter is ");
			Log::Logger()->log(Log::Info, std::to_string(text_source_[i]));
		}
	}

	text_source_ = new_text_source;
	Log::Logger()->log(Log::Debug, "Caesar::done text comfortable");

}

bool Caesar::prepare_to_modify(const std::string & key) {

	Log::Logger()->log(Log::Debug, "Caesar::preparing to modify");
	key_ = 0;

	std::istringstream sstream(key);
	int new_key = 0;
	if (!(sstream >> new_key) || (new_key < 0)) {
		return false;
	}

	text_to_lower();

	key_ = new_key;
	text_modified_ = "";
	Log::Logger()->log(Log::Debug, "Caesar::is ready to be modified");

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

	Log::Logger()->log(Log::Debug, "Caesar::tring to encrypt");
	std::size_t text_source_size = text_source_.length();
	for (std::size_t i = 0; i < text_source_size; ++i) {
		char letter_modified = (text_source_[i] - letter_first_ + key_) % (alphabet_len_)+letter_first_;
		text_modified_ += letter_modified;
	}
	Log::Logger()->log(Log::Debug, "Caesar::encrypting done");
}

void Caesar::decr() {

	Log::Logger()->log(Log::Debug, "Caesar::tring to decrypt");
	std::size_t text_source_size = text_source_.length();
	for (std::size_t i = 0; i < text_source_size; ++i) {
		int pos = (text_source_[i] - letter_first_ - key_);
		while (pos < 0) {
			pos += alphabet_len_;
		}
		char letter_modified = pos % (alphabet_len_)+letter_first_;
		text_modified_ += letter_modified;
	}
	Log::Logger()->log(Log::Debug, "Caesar::decrypting done");
}

Caesar::~Caesar() {

}
