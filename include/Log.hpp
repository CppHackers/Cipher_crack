#ifndef LOG_HPP
#define LOG_HPP

#include<fstream>
#include<string>

#define LOGFILE_NAME  "log_cipher.txt"

class Log {

public:

	static Log * Logger(unsigned level = Log::Debug);
	enum MSG { Debug, Info, Warn, Error };
	void log(unsigned level, const std::string & information);
	void set_log_level(unsigned level);
	~Log();

protected:

	Log(unsigned level);
	
private:

	static Log * logger_;
	unsigned level_;
	std::string log_file_name = LOGFILE_NAME;
	std::ofstream log_file_;
};

#endif // !LOG_HPP
