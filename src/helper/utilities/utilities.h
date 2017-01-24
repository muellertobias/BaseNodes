/*
 * utilities.h
 *
 *  Created on: 16.11.2016
 *      Author: tobias
 */

#ifndef HELPER_UTILITIES_UTILITIES_H_
#define HELPER_UTILITIES_UTILITIES_H_

#include <unistd.h>
#include <cctype>
#include <fstream>
#include <iterator>
#include <sstream>
#include <string>
#include <typeinfo>       // operator typeid

namespace helper {
namespace utilities {

static int localID = 0;

static inline bool isNumber(const std::string& str) {
	std::string::const_iterator it = str.begin();
	while (it != str.end() && std::isdigit(*it)) ++it;
	return !str.empty() && it == str.end();
}

static inline void writeLog(const std::string& log) {
	std::stringstream logfilename;
	logfilename << "log_" << localID << "_" << getpid() << ".txt";
	std::ofstream logFile;
	logFile.open(logfilename.str(), std::ios::app);
	logFile << log;
	logFile.close();
}

static inline void writeLog(const std::string& location, const std::exception& ex) {
	std::stringstream log;
	log << typeid(ex).name() << "- " << ex.what() << " in " << location << std::endl;
	writeLog(log.str());
}

static inline void writeLog(const std::string& location, const std::string& error) {
	std::stringstream log;
	log << error << " in " << location << std::endl;
	writeLog(log.str());
}

}
}



#endif /* HELPER_UTILITIES_UTILITIES_H_ */
