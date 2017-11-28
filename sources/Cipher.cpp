#include"Cipher.hpp"

Cipher::Cipher() {

	text_source_ = "";
	text_modified_ = "";
	log("Debug. Created Cipher");
}

void Cipher::text_source_in(std::istream & in) {

	log("Debug. Reading text_source from terminal");
	text_source_ = "";
	do {
		std::string new_text = "";
		std::getline(in, new_text);
		if (new_text.length() > 0) {
			text_source_ += new_text;
		} else {
			break;
		}
	} while (true);
	text_modified_ = "";
	log("Debug. Completed reading text_source from terminal");
}

void Cipher::text_modified_out(std::ostream & out) const {

	log("Debug. Output text_modified_ to terminal");
	out << text_modified_;
}

void Cipher::text_source_in(std::ifstream & in) {

	log("Debug. Reading text_source from file");
	text_source_ = "";
	do {
		std::string new_text = "";
		std::getline(in, new_text);
		if (new_text.length() > 0) {
			text_source_ += new_text;
		} else {
			break;
		}
	} while (true);
	text_modified_ = "";
	log("Debug. Completed reading text_source from file");

}

void Cipher::text_modified_out(std::ofstream & out) const {

	log("Debug. Output text_source to file");
	out << text_modified_;
}

void Cipher::set_text_source(std::string const & text_source) {

	log("Debug. Setting text_source in programm");
	text_source_ = text_source;
	text_modified_ = "";
}

std::string Cipher::get_text_source() const {

	log("Debug. Getting text_source in programm");
	return text_source_;
}

std::string Cipher::get_text_modified() const {

	log("Debug. Getting text_modified in programm");
	return text_modified_;
}

bool Cipher::is_modified() const {

	log("Debug. Is_modified request");
	return text_modified_.length() > 0;
}

void log(const std::string & information) {

	log_file_ << information << '\n';
}

Cipher::~Cipher() {

}