/*
 * ILogging.h
 *
 *  Created on: 19.11.2016
 *      Author: tobias
 */

#ifndef HELPER_LOGGING_ILOGGING_H_
#define HELPER_LOGGING_ILOGGING_H_

#include <string>

namespace helper {
namespace logging {

class ILogging {
public:
	virtual ~ILogging() { }
	void log(const std::string& content);
};

}
} /* namespace helper */

#endif /* HELPER_LOGGING_ILOGGING_H_ */
