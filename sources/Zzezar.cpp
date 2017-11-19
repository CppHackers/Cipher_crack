#include"Zzezar.hpp"

Zzezar::Zzezar(char letter_first)
	:
	cipher(),
	key_(0),
	letter_first_(letter_first),
	frequency_table_(nullptr),
	alphabet_(nullptr){

	switch (letter_first_) {
	case'a':
		//warning with locale
		alphabet_len_ = ENGLISH_ALPHABET_LEN;
		frequency_table_ = ENGLISH_LETTER_FREQUENCIES;
		alphabet_ = ENGLISH_ALPHABET;
		break;
	default:
		//err
		break;
	}

}

void Zzezar::encrypt(const std::string& key) {

	if (!prepare_to_modify(key)) {
		return;
	}
	encr();
}

void Zzezar::decrypt(const std::string& key) {

	if (!prepare_to_modify(key)) {
		return;
	}
	decr();
}

void Zzezar::crack() {

	//try to hold topN values of text or change koeffs
	float max_probability = 0;
	int cur_key = 0;
	int max_probabbility_key = 0;
	std::size_t * letters_count = new std::size_t[alphabet_len_];

	while (cur_key < alphabet_len_) {
		for (std::size_t i = 0; i < alphabet_len_; ++i) {
			letters_count[i] = 0;
		}
		decrypt(std::to_string(cur_key));
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

	decrypt(std::to_string(max_probabbility_key));
	delete[] letters_count;
}


///private

bool Zzezar::prepare_to_modify(const std::string & key) {

	if (text_source_.length() == 0) {
		return false;
	}
	// to destroy all errors
	std::istringstream sstream(key);
	int new_key = 0;
	if (!(sstream >> new_key) || (new_key < 0)) {
		//or very big key
		//error;
		return false;
	}

	//to lowwer all letters and destroy delimiters
	std::string new_text_source = "";
	for (std::size_t i = 0; i < text_source_.length(); ++i) {
		if (from_this_alphabet(tolower(text_source_[i]))) {
			new_text_source += tolower(text_source_[i]);
			//warning with locale
		}
	}

	text_source_ = new_text_source;
	key_ = new_key;
	text_modified_ = "";

	return true;
}

bool Zzezar::from_this_alphabet(char letter) {

	for (std::size_t i = 0; i < alphabet_len_; ++i) {
		if (letter == alphabet_[i]) {
			return true;
		}
	}

	return false;
}

void Zzezar::encr() {

	std::size_t text_source_size = text_source_.length();
	for (std::size_t i = 0; i < text_source_size; ++i) {
		// pass delimetrs and letters from another alphabet
		//to lowwer
		if (from_this_alphabet(text_source_[i])) {
			char letter_modified = (text_source_[i] - letter_first_ + key_) % (alphabet_len_)+letter_first_;
			text_modified_ += letter_modified;
		}
	}
}

void Zzezar::decr() {

	std::size_t text_source_size = text_source_.length();
	for (std::size_t i = 0; i < text_source_size; ++i) {
		// pass delimetrs and letters from another alphabet
		//to lowwer or to upper
		if (from_this_alphabet(text_source_[i])) {
			int pos = (text_source_[i] - letter_first_ - key_);
			while (pos < 0) {
				pos += alphabet_len_;
			}
			char letter_modified = pos % (alphabet_len_)+letter_first_;
			text_modified_ += letter_modified;
		}
	}
}