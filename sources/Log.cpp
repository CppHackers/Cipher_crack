#include "Log.hpp"

Log * Log::logger_ = nullptr;

Log::Log(unsigned level)
	:level_(level) {

	if (level > MSG::Error) {
		level_ = MSG::Error;
	}
	log_file_.open(log_file_name);
}

Log * Log::Logger(unsigned level) {
	if (logger_ == nullptr) {
		logger_ = new Log(level);
	}
	return logger_;
}

void Log::log(unsigned level, const std::string & information) {

	if (level < level_) {
		return;
	}

	switch (level) {
	case Debug:
		log_file_ << "[Debug]: ";
		break;
	case Info:
		log_file_ << "[Info]: ";
		break;
	case Warn:
		log_file_ << "[Warn]: ";
		break;
	case Error:
		log_file_ << "[Error]: ";
		break;
	default:
		return;
	}
	log_file_ << information << "\n";
	
}

void Log::set_log_level(unsigned level) {

	level_ = level;
}

Log::~Log() {
	logger_->log_file_.close();
	delete logger_;

}

