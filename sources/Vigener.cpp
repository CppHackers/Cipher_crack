#include"Vigener.hpp"
#include"Caesar.hpp"


Vigener::Vigener(char letter_first, std::size_t max_key_len, bool need_spaces) 
	:
	Cipher(),
	key_(""),
	letter_first_(letter_first),
	key_len_(0),
	match_index_(0.0),
	frequency_table_(nullptr),
	alphabet_(nullptr),
	max_key_len_(max_key_len),
	need_spaces_(need_spaces){

	log("Debug. Vigener::creating Vigener");
	log("Info.  Vigener::first letter of alphabet: ");
	log(std::to_string(letter_first));
	switch (letter_first_) {
	case'a':
		alphabet_len_ = ENGLISH_ALPHABET_LEN;
		frequency_table_ = ENGLISH_LETTER_FREQUENCIES;
		alphabet_ = ENGLISH_ALPHABET;
		match_index_ = ENGLISH_MATCH_INDEX;
		break;
	default:
		log("Fatall. Vigener::has no this alphabet");
		throw std::invalid_argument("Invalid alphabet");
		break;
	}
	log("Debug. Vigener::created");

}

void Vigener::change_spaces_mod() {

	need_spaces_ = !need_spaces_;
}

void Vigener::encrypt(const std::string& key) {

	log("Debug. Vigener::want to encrypt");
	if (!prepare_to_modify(key)) {
		throw std::invalid_argument("Invalid key");
	}
	encr();
	if (need_spaces_) {
		spaces_reborn();
	}
}

void Vigener::decrypt(const std::string& key) {

	log("Debug. Vigener::want to decrypt");
	if (!prepare_to_modify(key)) {
		throw std::invalid_argument("Invalid key");
	}
	decr();
	if (need_spaces_) {
		spaces_reborn();
	}
}

void Vigener::crack() {

	log("Debug. Vigener::trying to crack");
	key_ = "";
	key_len_ = 0;
	text_modified_ = "";
	spaces_pos_.clear();
	text_to_lower();

	if (text_source_.length() < 2) {
		log("Error. Vigener::text is too short to crack");
		log("Debug. Vigener::refused to crack");
		throw(std::logic_error("Text is too small to crack"));
		return;
	}

	key_len_ = find_key_len();
	text_modified_ = find_text();
	if (need_spaces_) {
		spaces_reborn();
	}
	

	log("Debug. Vigener::cracking is completed");
}

std::string Vigener::get_key() const {

	return key_;
}

void Vigener::text_to_lower() {

	log("Debug. Vigener::doing text comfortable to work with");
	std::string new_text_source = "";
	std::size_t text_source_size = text_source_.length();
	for (std::size_t i = 0; i < text_source_size; ++i) {
		if (from_this_alphabet(tolower(text_source_[i]))) {
			new_text_source += tolower(text_source_[i]);
		} else {
			if (need_spaces_) {
				spaces_pos_.insert(i);
			}
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
	spaces_pos_.clear();

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

	if (text_source_.length() < 2) {

		log("Error. Vigener::text is too short to crack");
		throw(std::logic_error("Text is too small"));
	}

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

std::size_t Vigener::find_key_len() {

	log("Debug. Vigener::trying to find key_len");
	std::size_t text_len = text_source_.length();
	std::size_t prob_key_len = 1;
	std::size_t cur_key_len = 1;
	double max_prob_match_index = 0;

	std::size_t * letters_count = new std::size_t[alphabet_len_];
	while (cur_key_len <= max_key_len_) {
		double cur_match_index = 0;
		for (std::size_t i = 0; i < alphabet_len_; ++i) {
			letters_count[i] = 0;
		}

		std::size_t part_len = 0;
		for (std::size_t i = cur_key_len - 1; i < text_len; i += cur_key_len) {
			++part_len;
			letters_count[text_source_[i] - letter_first_] += 1;
		}

		for (std::size_t i = 0; i < alphabet_len_; ++i) {
			if (letters_count[i] > 1) {
				cur_match_index += letters_count[i] * (letters_count[i] - 1) *1.0 / (part_len * (part_len - 1));
			}
		}

		double dif_cur = (match_index_ - cur_match_index);
		double dif_min = (match_index_ - max_prob_match_index);
		if (dif_cur * dif_cur < dif_min * dif_min) {
			max_prob_match_index = cur_match_index;
			prob_key_len = cur_key_len;
		}

		++cur_key_len;
	}
	delete[] letters_count;
	log("Debug. Vigener::completed finding key_len");
	return prob_key_len;
}

std::string Vigener::find_text() {

	log("Debug. Vigener::trying to find real text");
	std::string * text_parts = new std::string[key_len_];
	std::size_t text_len = text_source_.length();
	for (std::size_t i = 0; i < key_len_; ++i) {
		text_parts[i] = "";
	}
	for (std::size_t i = 0; i < text_len; ++i) {
		text_parts[i % key_len_] += text_source_[i];
	}

	Caesar caesar_cracker(letter_first_);
	
	for (std::size_t i = 0; i < key_len_; ++i) {
		caesar_cracker.set_text_source(text_parts[i]);
		caesar_cracker.crack();
		char letter_source = text_parts[i][0];
		text_parts[i] = caesar_cracker.get_text_modified();
		key_ += find_key_letter(letter_source, text_parts[i][0]);
	}

	std::string text_modified = "";
	std::size_t max_text_part_len = text_len / key_len_ + (((text_len % key_len_) != 0) ? 1 : 0);
	for (std::size_t i = 0; i < max_text_part_len; ++i) {
		for (std::size_t j = 0; j < key_len_; ++j) {
			if (text_parts[j].length() < i) {
				break;
			}
			text_modified += text_parts[j][i];
		}
	}

	delete[] text_parts;
	log("Debug. Vigener::completed finding real text");
	return text_modified;
}

char Vigener::find_key_letter(char letter_source, char letter_modified) {

	log("Debug. Vigener::trying to find key_letter");
	
	int pos = letter_source - letter_modified;

	if (pos < 0) {
		pos += alphabet_len_;
	}
	log("Debug. Vigener::completed finding key_letter");

	return letter_first_ + pos;

}

Vigener::~Vigener() {

}

void Vigener::spaces_reborn() {

	if (spaces_pos_.size() > 0) {
		std::size_t new_text_len = text_modified_.length() + spaces_pos_.size();
		std::size_t spaces_count = 0;
		std::string new_text_modified = "";
		auto end = spaces_pos_.end();
		for (std::size_t i = 0; i < new_text_len; ++i) {
			if (spaces_pos_.find(i) != end) {
				new_text_modified += " ";
				++spaces_count;
			} else {
				new_text_modified += text_modified_[i - spaces_count];
			}
		}

		text_modified_ = new_text_modified;
	}
}