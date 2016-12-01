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
#include "NodeBaseException.h"

namespace helper {
namespace exception {

class ConfigurationException: public NodeBaseException {
public:
	ConfigurationException(const std::string& msg) : NodeBaseException(msg) { }
	virtual ~ConfigurationException() { }
};

}
} /* namespace helper */

#endif /* HELPER_EXCEPTION_CONFIGURATIONEXCEPTION_H_ */
