/*
 * Logger.h
 *
 *  Created on: 22.11.2016
 *      Author: tobias
 */

#ifndef HELPER_LOGGING_LOGGER_H_
#define HELPER_LOGGING_LOGGER_H_
#include <fstream>
#include <iostream>
#include <string>

namespace helper {
namespace logging {

using namespace std;

class Logger {
public:
	static Logger* getInstance();

	void setFilename(const string& filename);
	void log(const string& text);

private:

	static Logger* instance;
	ofstream logFile;
	string filename;

	Logger();
	Logger(const Logger&){};
	Logger& operator=(const Logger&) { return *this;};

	void openFileIfNotOpen();
};

} /* namespace logging */
} /* namespace helper */

#endif /* HELPER_LOGGING_LOGGER_H_ */
