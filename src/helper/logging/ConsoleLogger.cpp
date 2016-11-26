/*
 * ConsoleLogger.cpp
 *
 *  Created on: 19.11.2016
 *      Author: tobias
 */

#include "ConsoleLogger.h"
#include <iostream>

namespace helper {
namespace logging {
ConsoleLogger::ConsoleLogger() {
}

ConsoleLogger::~ConsoleLogger() {
}

void ConsoleLogger::log(const string& content) {
	cout << content << endl;
}

}
} /* namespace helper */
