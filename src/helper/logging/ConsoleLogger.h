/*
 * ConsoleLogger.h
 *
 *  Created on: 19.11.2016
 *      Author: tobias
 */

#ifndef HELPER_LOGGING_CONSOLELOGGER_H_
#define HELPER_LOGGING_CONSOLELOGGER_H_

#include "ILogging.h"

using namespace std;

namespace helper {
namespace logging {

class ConsoleLogger: public ILogging {
public:
	ConsoleLogger();
	virtual ~ConsoleLogger();

	void log(const string& content);
};

}
} /* namespace helper */

#endif /* HELPER_LOGGING_CONSOLELOGGER_H_ */
