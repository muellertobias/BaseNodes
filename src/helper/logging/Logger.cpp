/*
 * Logger.cpp
 *
 *  Created on: 22.11.2016
 *      Author: tobias
 */

#include "Logger.h"

#include <cstdio>

namespace helper {
namespace logging {

Logger* Logger::instance = nullptr;

Logger::Logger() {
	this->filename = "log.txt"; // default
}

Logger* Logger::getInstance() {
	if (instance == nullptr) {
		instance = new Logger();
	}
	return instance;
}

void Logger::setFilename(const string& newFilename) {
	if (logFile.is_open()) {
		logFile.close();
		ifstream oldFile;
		string buffer;
		oldFile.open(this->filename);
		logFile.open(newFilename, ios::out | ios::app);
		while (oldFile.eof()) {
			getline(oldFile, buffer);
			logFile << buffer << endl;
		}
	}
	this->filename = newFilename;
}

void Logger::log(const string& text) {
	openFileIfNotOpen();

	logFile << text << endl;
}

void Logger::openFileIfNotOpen() {
	if (!logFile.is_open()) {
		logFile.open(filename, ios::out | ios::app);
	}
}

} /* namespace logging */
} /* namespace helper */

