/*
 * NetworkException.h
 *
 *  Created on: 01.12.2016
 *      Author: tobias
 */

#ifndef HELPER_EXCEPTION_NETWORKEXCEPTION_H_
#define HELPER_EXCEPTION_NETWORKEXCEPTION_H_

#include <exception>
#include <string>
#include "NodeBaseException.h"


namespace helper {
namespace exception {

class NetworkException: public NodeBaseException {
public:
	NetworkException(const std::string& msg) : NodeBaseException(msg) { }
	virtual ~NetworkException() { }
};

}
} /* namespace helper */

#endif /* HELPER_EXCEPTION_NETWORKEXCEPTION_H_ */
