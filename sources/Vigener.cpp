#include"Vigener.hpp"
#include"Caesar.hpp"

Vigener::Vigener(char letter_first, std::size_t max_key_len,
	bool need_spaces, unsigned log_level,
	unsigned top_key_len, unsigned top_key) 
	:
	Cipher(log_level),
	key_(""),
	letter_first_(letter_first),
	key_len_(0),
	match_index_(0.0),
	frequency_table_(nullptr),
	alphabet_(nullptr),
	max_key_len_(max_key_len),
	need_spaces_(need_spaces),
	top_key_len_(top_key_len),
	top_key_(top_key) {

	Log::Logger()->log(Log::Debug, "Vigener::creating Vigener");
	Log::Logger()->log(Log::Info, " Vigener::first letter of alphabet: ");
	Log::Logger()->log(Log::Info, std::to_string(letter_first));
	switch (letter_first_) {
	case'a':
		alphabet_len_ = ENGLISH_ALPHABET_LEN;
		frequency_table_ = ENGLISH_LETTER_FREQUENCIES;
		alphabet_ = ENGLISH_ALPHABET;
		match_index_ = ENGLISH_MATCH_INDEX;
		break;
	default:
		Log::Logger()->log(Log::Error, "Fatall. Vigener::has no this alphabet");
		throw std::invalid_argument("Invalid alphabet");
		break;
	}
	letters_count_ = new std::size_t[alphabet_len_];
	Log::Logger()->log(Log::Debug, "Vigener::created");

}

void Vigener::change_spaces_mode() {

	need_spaces_ = !need_spaces_;
}

void Vigener::encrypt(const std::string& key) {

	Log::Logger()->log(Log::Debug, "Vigener::want to encrypt");
	if (!prepare_to_modify(key)) {
		throw std::invalid_argument("Invalid key");
	}
	encr();
	if (need_spaces_) {
		text_modified_ = spaces_reborn(text_modified_);
	}
}

void Vigener::decrypt(const std::string& key) {

	Log::Logger()->log(Log::Debug, "Vigener::want to decrypt");
	if (!prepare_to_modify(key)) {
		Log::Logger()->log(Log::Error, "Vigener::bad key");
		Log::Logger()->log(Log::Info, " Vigener::key is");
		Log::Logger()->log(Log::Info, key);
		throw std::invalid_argument("Invalid key");
	}
	decr();
	if (need_spaces_) {
		text_modified_ = spaces_reborn(text_modified_);
	}
}

void Vigener::crack() {

	std::size_t min_text_len = 2;

	Log::Logger()->log(Log::Debug, "Vigener::trying to crack");
	key_ = "";
	key_len_ = 0;
	text_modified_ = "";
	if (top_key_len_) {
		top_vector_.clear();
		top_vector_.assign((top_key_len_ < max_key_len_) ? top_key_len_ : max_key_len_, decryptod_Vigener_mod(0, 0.0, top_key_));
	}
	spaces_pos_.clear();
	text_to_lower();

	if (text_source_.length() < min_text_len) {
		Log::Logger()->log(Log::Error, "Vigener::text is too short to crack");
		Log::Logger()->log(Log::Debug, "Vigener::refused to crack");
		throw(std::logic_error("Text is too small to crack"));
		return;
	}


	key_len_ = find_key_len();
	if (top_key_len_ && top_key_) {
		text_modified_ = find_top_text();
		key_ = top_vector_[0].top_key_vector_[0].key_;
	} else {
		text_modified_ = find_text();
	}
	if (need_spaces_) {
		text_modified_ = spaces_reborn(text_modified_);
	}
	Log::Logger()->log(Log::Debug, "Vigener::cracking is completed");
}

std::string Vigener::get_key() const {

	return key_;
}

void Vigener::text_to_lower() {

	Log::Logger()->log(Log::Debug, "Vigener::doing text comfortable to work with");
	std::string new_text_source = "";
	std::size_t text_source_size = text_source_.length();
	for (std::size_t i = 0; i < text_source_size; ++i) {
		if (from_this_alphabet(tolower(text_source_[i]))) {
			new_text_source += tolower(text_source_[i]);
		} else {
			if (need_spaces_) {
				spaces_pos_.insert(i);
			}
			Log::Logger()->log(Log::Warn, " Vigener::letter is not from selected alphabet");
			Log::Logger()->log(Log::Info, " Vigener::letter is ");
			Log::Logger()->log(Log::Info, std::to_string(text_source_[i]));
		}
	}

	text_source_ = new_text_source;
	Log::Logger()->log(Log::Debug, "Vigener::done text comfortable");

}

bool Vigener::prepare_to_modify(const std::string & key) {

	std::size_t min_text_len = 2;

	Log::Logger()->log(Log::Debug, "Vigener::preparing to modify");
	key_ = "";
	key_len_ = 0;
	spaces_pos_.clear();

	std::size_t key_len = key.length();
	if (key_len == 0) {
		return false;
	}

	std::string new_key = "";
	for (std::size_t i = 0; i < key_len; ++i) {
		if (from_this_alphabet(tolower(key[i]))) {
			new_key += tolower(key[i]);
		} else {
			return false;
		}
	}

	text_to_lower();

	if (text_source_.length() < min_text_len) {
		Log::Logger()->log(Log::Error, "Vigener::text is too short to crack");
		throw(std::logic_error("Text is too small"));
	}

	key_len_ = key_len;
	key_ = new_key;
	text_modified_ = "";
	Log::Logger()->log(Log::Debug, "Vigener::is ready to be modified");

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

	Log::Logger()->log(Log::Debug, "Vigener::tring to encrypt");
	std::size_t text_source_size = text_source_.length();
	for (std::size_t i = 0; i < text_source_size; ++i) {
		char letter_modified = (text_source_[i] - letter_first_ + (key_[i % key_len_] - letter_first_)) % (alphabet_len_)+letter_first_;
		text_modified_ += letter_modified;
	}
	Log::Logger()->log(Log::Debug, "Vigener::encrypting done");
}

void Vigener::decr() {

	Log::Logger()->log(Log::Debug, "Vigener::tring to decrypt");
	std::size_t text_source_size = text_source_.length();
	for (std::size_t i = 0; i < text_source_size; ++i) {
		int pos = (text_source_[i] - letter_first_ - (key_[i % key_len_] - letter_first_));
		while (pos < 0) {
			pos += alphabet_len_;
		}
		char letter_modified = pos % (alphabet_len_) + letter_first_;
		text_modified_ += letter_modified;
	}
	Log::Logger()->log(Log::Debug, "Vigener::decrypting done");
}

std::size_t Vigener::find_key_len() {

	Log::Logger()->log(Log::Debug, "Vigener::trying to find key_len");
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

		if (top_key_len_) {
			add_decryptod_Vigener_mod(decryptod_Vigener_mod(cur_key_len, cur_match_index, top_key_));
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
	Log::Logger()->log(Log::Debug, "Vigener::completed finding key_len");
	return prob_key_len;
}

std::string Vigener::find_text() {

	Log::Logger()->log(Log::Debug, "Vigener::trying to find real text");
	std::string * text_parts = new std::string[key_len_];
	std::size_t text_len = text_source_.length();
	for (std::size_t i = 0; i < key_len_; ++i) {
		text_parts[i] = "";
	}
	for (std::size_t i = 0; i < text_len; ++i) {
		text_parts[i % key_len_] += text_source_[i];
	}

	Caesar caesar_cracker(letter_first_, false, Log::Error, 0);
	
	for (std::size_t i = 0; i < key_len_; ++i) {
		caesar_cracker.set_text_source(text_parts[i]);
		caesar_cracker.crack();
		char letter_source = text_parts[i][0];
		text_parts[i] = caesar_cracker.get_text_modified();
		key_ += find_key_letter(letter_source, text_parts[i][0]);
	}

	std::string text_modified = "";
	std::size_t max_text_part_len = text_len / key_len_ + (((text_len % key_len_) != 0) ? 1 : 0);
	std::size_t count_letters = 0;
	for (std::size_t i = 0; i < max_text_part_len; ++i) {
		for (std::size_t j = 0; j < key_len_; ++j) {
			/*
			if (text_parts[j].length() < i) {
				break;
			}
			*/
			if (count_letters == text_len) {
				break;
			}
			++count_letters;
			text_modified += text_parts[j][i];
		}
	}

	delete[] text_parts;
	Log::Logger()->log(Log::Debug, "Vigener::completed finding real text");
	return text_modified;
}

std::string Vigener::find_top_text() {

	//top_key for every top_key_len

	Log::Logger()->log(Log::Debug, "Vigener::trying to find top of real text");
	Caesar caesar_cracker(letter_first_, false, Log::Error, top_key_);
	std::size_t text_len = text_source_.length();
	std::size_t top_key_len = top_vector_.size(); // or (top_key_len_ < max_key_len_) ? top_key_len_ : max_key_len_
	for (std::size_t i = 0; i < top_key_len; ++i) {
		std::size_t cur_key_len = top_vector_[i].key_len_;
		std::string * text_parts = new std::string[cur_key_len];
		for (std::size_t j = 0; j < cur_key_len; ++j) {
			text_parts[j] = "";
		}
		for (std::size_t j = 0; j < text_len; ++j) {
			text_parts[j % cur_key_len] += text_source_[j];
		}

		std::vector<std::vector<decryptod<int>>> decr_caesar_top_vector; // matrix top_key_len_ : top_key
		std::vector<char> letters_source;
		for (std::size_t j = 0; j < cur_key_len; ++j) {
			caesar_cracker.set_text_source(text_parts[j]);
			caesar_cracker.crack();
			decr_caesar_top_vector.push_back(caesar_cracker.get_top());
			letters_source.push_back(text_parts[j][0]);
		}
		
		std::size_t top_key = top_vector_[i].top_key_vector_.size(); // or top_key_ < pow(alphabet_len_, cur_key_len) ? top_key_ : pow()
		std::vector<std::size_t> indexes;//size==cur_key_len
		find_optimal_top(indexes, decr_caesar_top_vector, letters_source, text_len, text_parts, top_key, top_key_len, cur_key_len, top_vector_[i], 0);
		delete[] text_parts;
	}
	Log::Logger()->log(Log::Debug, "Vigener::completed finding top of real text");
	
	return top_vector_[0].top_key_vector_[0].text_;
}

Vigener::~Vigener() {
	delete[] letters_count_;
}

std::string Vigener::spaces_reborn(std::string const & text_modified) {

	Log::Logger()->log(Log::Debug, "Vigener::trying to reborn spaces");
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
		Log::Logger()->log(Log::Debug, "Vigener::complete spaces reborn");
		return new_text_modified;
	}

	Log::Logger()->log(Log::Debug, "Vigener::complete spaces reborn, but don't find it");
	return text_modified;
}

void Vigener::add_decryptod_Vigener_mod(decryptod_Vigener_mod decr) {

	std::size_t top_len = top_vector_.size();
	bool find_place = false;
	for (std::size_t i = 0; i < top_len; ++i) {
		if (top_vector_[i].match_index_ < decr.match_index_) {
			find_place = true;
		}
		if (find_place) {
			decryptod_Vigener_mod old_decr = top_vector_[i];
			top_vector_[i] = decr;
			decr = old_decr;
		}
	}
}

void Vigener::find_optimal_top(std::vector<std::size_t> & indexes, std::vector<std::vector<decryptod<int>>> & source_variant,
	std::vector<char> & letters_source, std::size_t text_len, std::string *& text_parts,
	std::size_t top_key, std::size_t top_key_len, std::size_t cur_key_len,
	decryptod_Vigener_mod & cur_decr, std::size_t deep) {

	if (deep == cur_key_len) {

		std::string text_modified = "";
		std::size_t max_text_part_len = text_len / cur_key_len + (((text_len % cur_key_len) != 0) ? 1 : 0);
		std::string key = "";
		for (std::size_t i = 0; i < cur_key_len; ++i) {
			text_parts[i] = source_variant[i][indexes[i]].text_;
			key += find_key_letter(letters_source[i], text_parts[i][0]); // ?
		}
		std::size_t count_letters = 0;
		for (std::size_t i = 0; i < max_text_part_len; ++i) {
			for (std::size_t j = 0; j < cur_key_len; ++j) {
				/*
				if (text_parts[j].length() < i) {
					break;
				}
				*/
				if (count_letters == text_len) {
					break;
				}
				++count_letters;
				text_modified += text_parts[j][i];
			}
		}
		double factor = calc_factor(text_modified); // ?
		add_decryptod(cur_decr , decryptod<std::string>( text_modified, key, factor )); // ?
		indexes.pop_back();
	} else {
		for (std::size_t i = 0; i < top_key; ++i) {
			indexes.push_back(i);
			find_optimal_top(indexes, source_variant, letters_source, text_len, text_parts, top_key, top_key_len, cur_key_len, cur_decr, deep + 1);
		}
	}
}

char Vigener::find_key_letter(char letter_source, char letter_modified) {

	Log::Logger()->log(Log::Debug, "Vigener::trying to find key_letter");

	int pos = letter_source - letter_modified;

	if (pos < 0) {
		pos += alphabet_len_;
	}
	Log::Logger()->log(Log::Debug, "Vigener::completed finding key_letter");

	return letter_first_ + pos;

}

void Vigener::add_decryptod(decryptod_Vigener_mod & decr_V_m, decryptod<std::string> decr_C) {

	std::size_t top_key = decr_V_m.top_key_vector_.size();
	bool find_place = false;
	for (std::size_t i = 0; i < top_key; ++i) {
		if (decr_V_m.top_key_vector_[i].text_.compare(decr_C.text_) == 0) {
			break;
		}
		if (decr_V_m.top_key_vector_[i].factor_ < decr_C.factor_) {
			find_place = true;
		}
		if (find_place) {
			decryptod<std::string> old_decr = decr_V_m.top_key_vector_[i];
			decr_V_m.top_key_vector_[i] = decr_C;
			decr_C = old_decr;
		}
	}

}

double Vigener::calc_factor(std::string const & text) {

	
	for (std::size_t i = 0; i < alphabet_len_; ++i) {
		letters_count_[i] = 0;
	}
	std::size_t max_letters = text.length();
	for (std::size_t i = 0; i < max_letters; ++i) {
		letters_count_[text[i] - letter_first_] += 1;
	}
	double cur_probability = 0;
	for (std::size_t i = 0; i < alphabet_len_; ++i) {
		cur_probability += ((letters_count_[i] * 1.0) / max_letters)  * (frequency_table_[i] / 100);
	}
	return cur_probability;
}

// +output

std::ostream & Vigener::top_out(std::ostream & out) {

	if (!key_len_) {
		return out;
	}
	std::size_t top_key_len = top_vector_.size(); 
	for (std::size_t i = top_key_len; i > 0; --i) {
		std::size_t top_key = top_vector_[i - 1].top_key_vector_.size();
		std::cout << "Key_len: " << top_vector_[i - 1].key_len_ << "\n";
		std::cout << "Match_index: " << top_vector_[i - 1].match_index_ << "\n\n";

		for (std::size_t j = top_key; j > 0; --j) {
			if (need_spaces_) {
				top_vector_[i - 1].top_key_vector_[j - 1].text_ = spaces_reborn(top_vector_[i - 1].top_key_vector_[j - 1].text_);
			}
			out << i  << "." << j  << ".\n";
			out << "Text:\n" << top_vector_[i - 1].top_key_vector_[j - 1].text_ << "\n";
			out << "Key: " << top_vector_[i - 1].top_key_vector_[j - 1].key_ << "\n";
			out << "Factor: " << top_vector_[i - 1].top_key_vector_[j - 1].factor_ << "\n\n";
		}
	}

	return out;
}

std::ofstream & Vigener::top_out(std::ofstream & out) {

	if (!key_len_) {
		return out;
	}
	std::size_t top_key_len = top_vector_.size(); 
	for (std::size_t i = 0; i < top_key_len; ++i) {
		std::size_t top_key = top_vector_[i].top_key_vector_.size();
		out << "Key_len: " << top_vector_[i].key_len_ << "\n";
		out << "Match_index: " << top_vector_[i].match_index_ << "\n";

		for (std::size_t j = 0; j < top_key; ++j) {
			if (need_spaces_) {
				top_vector_[i].top_key_vector_[j].text_ = spaces_reborn(top_vector_[i].top_key_vector_[j].text_);
			}
			out << i + 1 << "." << j + 1 << ".\n";
			out << "Text:\n" << top_vector_[i].top_key_vector_[j].text_ << "\n";
			out << "Key: " << top_vector_[i].top_key_vector_[j].key_ << "\n";
			out << "Factor: " << top_vector_[i].top_key_vector_[j].factor_ << "\n\n";
		}
	}

	return out;
}