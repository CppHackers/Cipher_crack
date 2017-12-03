#include"Cipher.hpp"

Cipher::Cipher(unsigned log_level) {

	text_source_ = "";
	text_modified_ = "";
	Log::Logger(log_level)->log(Log::Debug, "Created Cipher");
}

void Cipher::text_source_in(std::istream & in) {

	Log::Logger()->log(Log::Debug, "Reading text_source from terminal");
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
	Log::Logger()->log(Log::Debug, "Completed reading text_source from terminal");
}

void Cipher::text_modified_out(std::ostream & out) const {

	Log::Logger()->log(Log::Debug, "Output text_modified_ to terminal");
	out << text_modified_;
}

void Cipher::text_source_in(std::ifstream & in) {

	Log::Logger()->log(Log::Debug, "Reading text_source from file");
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
	Log::Logger()->log(Log::Debug, "Completed reading text_source from file");

}

void Cipher::text_modified_out(std::ofstream & out) const {

	Log::Logger()->log(Log::Debug, "Output text_source to file");
	out << text_modified_;
}

void Cipher::set_text_source(std::string const & text_source) {

	Log::Logger()->log(Log::Debug, "Setting text_source in programm");
	text_source_ = text_source;
	text_modified_ = "";
}

std::string Cipher::get_text_source() const {

	Log::Logger()->log(Log::Debug, "Getting text_source in programm");
	return text_source_;
}

std::string Cipher::get_text_modified() const {

	Log::Logger()->log(Log::Debug, "Getting text_modified in programm");
	return text_modified_;
}

bool Cipher::is_modified() const {

	Log::Logger()->log(Log::Debug, "Is_modified request");
	return text_modified_.length() > 0;
}


Cipher::~Cipher() {

}