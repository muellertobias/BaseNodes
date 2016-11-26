/*
 * ConfigurationException.h
 *
 *  Created on: 30.10.2016
 *      Author: tobias
 */

#ifndef HELPER_EXCEPTION_CONFIGURATIONEXCEPTION_H_
#define HELPER_EXCEPTION_CONFIGURATIONEXCEPTION_H_

#include <exception>
#include <string>

namespace helper {
namespace exception {

class ConfigurationException: public std::exception {
public:
	ConfigurationException(const std::string& msg) : message(msg) { }
	virtual ~ConfigurationException() { }

	virtual const char* what() const noexcept{
		return message.c_str();
	}
private:
	const std::string& message;
};

}
} /* namespace helper */

#endif /* HELPER_EXCEPTION_CONFIGURATIONEXCEPTION_H_ */
