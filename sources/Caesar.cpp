#include"Caesar.hpp"

Caesar::Caesar(char letter_first, bool need_spaces, unsigned log_level, unsigned top)
	:
	Cipher(log_level),
	key_(0),
	letter_first_(letter_first),
	frequency_table_(nullptr),
	alphabet_(nullptr),
	need_spaces_(need_spaces),
	top_(top) {

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
	
	letters_count_ = new std::size_t[alphabet_len_];
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
	if (need_spaces_) {
		text_modified_ = spaces_reborn(text_modified_);
	}
}

void Caesar::decrypt(const std::string& key) {

	Log::Logger()->log(Log::Debug, "Caesar::want to decrypt");
	if (!prepare_to_modify(key)) {
		throw std::invalid_argument("Invalid key");
	}
	decr();
	if (need_spaces_) {
		text_modified_ = spaces_reborn(text_modified_);
	}
}

void Caesar::crack() {

	Log::Logger()->log(Log::Debug, "Caesar::Trying to crack");
	if (top_) {
		top_vector_.clear();
		top_vector_.assign((top_ < alphabet_len_) ? top_ : alphabet_len_, decryptod<int>());
	}
	spaces_pos_.clear();
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
	

	while (cur_key < alphabet_len_) {

		key_ = cur_key;
		text_modified_ = "";
		decr();

		for (std::size_t i = 0; i < alphabet_len_; ++i) {
			letters_count_[i] = 0;
		}
		std::size_t max_letters = text_modified_.length();
		for (std::size_t i = 0; i < max_letters; ++i) {
			letters_count_[text_modified_[i] - letter_first_] += 1;
		}

		double cur_probability = 0;
		for (std::size_t i = 0; i < alphabet_len_; ++i) {
			cur_probability += ((letters_count_[i] * 1.0) / max_letters )  * (frequency_table_[i] / 100);
		}
		if (top_) {
			add_decryptod({ text_modified_, cur_key, cur_probability });
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
	if (need_spaces_) {
		text_modified_ = spaces_reborn(text_modified_);
	}
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
			if (need_spaces_) {
				spaces_pos_.insert(i);
			}
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
	spaces_pos_.clear();

	std::istringstream sstream(key);
	int new_key = 0;
	if (!(sstream >> new_key)) {
		return false;
	}

	text_to_lower();

	while(new_key < 0) {
		new_key += alphabet_len_;
	}

	key_ = new_key % alphabet_len_;
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
	delete[] letters_count_;
}

std::string Caesar::spaces_reborn(std::string const & text_modified) {

	Log::Logger()->log(Log::Debug, "Caesar::trying to reborn spaces");
	if (spaces_pos_.size() > 0) {
		std::size_t new_text_len = text_modified.length() + spaces_pos_.size();
		std::size_t spaces_count = 0;
		std::string new_text_modified = "";
		auto end = spaces_pos_.end();
		for (std::size_t i = 0; i < new_text_len; ++i) {
			if (spaces_pos_.find(i) != end) {
				new_text_modified += " ";
				++spaces_count;
			} else {
				new_text_modified += text_modified[i - spaces_count];
			}
		}
		Log::Logger()->log(Log::Debug, "Caesar::complete spaces reborn");
		return new_text_modified;
	}

	Log::Logger()->log(Log::Debug, "Caesar::complete spaces reborn, but don't find it");
	return text_modified;
}

void Caesar::change_spaces_mode() {

	need_spaces_ = !need_spaces_;
}

void Caesar::add_decryptod(decryptod<int> decr) {

	std::size_t top_len = top_vector_.size(); // or (top_ < alphabet_len) ? top_ : alphabet_len_
	bool find_place = false;
	for (std::size_t i = 0; i < top_len; ++i) {
		if (top_vector_[i].factor_ < decr.factor_) {
			find_place = true;
		}
		if (find_place) {
			decryptod<int> old_decr = top_vector_[i];
			top_vector_[i] = decr;
			decr = old_decr;
		}
	}
}

std::vector<decryptod<int>> Caesar::get_top() {

	return top_vector_;
}

//+out

std::ostream & Caesar::top_out(std::ostream & out) {

	if (!top_) {
		return out;
	}

	std::size_t top_len = top_vector_.size(); // top_ or alphabet_len_
	for (std::size_t i = top_len; i > 0; --i) {

		if (need_spaces_) {
			top_vector_[i - 1].text_ = spaces_reborn(top_vector_[i - 1].text_);
		}

		out << i << ".\n";
		out << "Text:\n" << top_vector_[i - 1].text_ << "\n";
		out << "Key: " << top_vector_[i - 1].key_ << "\n";
		out << "Factor: " << top_vector_[i - 1].factor_ << "\n\n";
	}

	return out;
}

std::ofstream & Caesar::top_out(std::ofstream & out) {

	if (!top_) {
		return out;
	}

	std::size_t top_len = top_vector_.size(); // top_ or alphabet_len_
	for (std::size_t i = 0; i < top_len; ++i) {

		if (need_spaces_) {
			top_vector_[i].text_ = spaces_reborn(top_vector_[i].text_);
		}

		out << i + 1 << ".\n";
		out << "Text:\n" << top_vector_[i].text_ << "\n";
		out << "Key: " << top_vector_[i].key_ << "\n";
		out << "Factor: " << top_vector_[i].factor_ << "\n\n";
	}

	return out;
}

