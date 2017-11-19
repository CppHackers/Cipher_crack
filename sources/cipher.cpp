#include"cipher.hpp"

cipher::cipher() {

	text_source_ = "";
	text_modified_ = "";
}

void cipher::text_source_in(std::istream & in) {

	//tolower if you want, but warning with lacale
	//destroy all delimiters if you want
	text_source_ = "";
	do {
		//reading strings of text and to gather in one big string
		std::string new_text = "";
		std::getline(in, new_text);
		if (new_text.length() > 0) {
			text_source_ += new_text;
		} else {
			break;
		}
	} while (true);
	text_modified_ = "";
}

void cipher::text_modified_out(std::ostream & out) const {

	out << text_modified_;
}

void cipher::text_source_in(std::ifstream & in) {

	//tolower if you want, but warning with lacale
	//destroy all delimiters, if you want
	text_source_ = "";
	do {
		//reading strings of text and to gather in one big string
		std::string new_text = "";
		std::getline(in, new_text);
		if (new_text.length() > 0) {
			text_source_ += new_text;
		} else {
			break;
		}
	} while (true);
	text_modified_ = "";
}

void cipher::text_modified_out(std::ofstream & out) const {

	out << text_modified_;
}

void cipher::set_text_source(std::string const & text_source) {

	text_source_ = text_source;
	text_modified_ = "";
}

std::string cipher::get_text_source() const {

	return text_source_;
}

std::string cipher::get_text_modified() const {

	return text_modified_;
}

bool cipher::is_modified() const {

	return text_modified_.length() > 0;
}
