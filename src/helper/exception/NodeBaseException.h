/*
 * NodeBaseException.h
 *
 *  Created on: 01.12.2016
 *      Author: tobias
 */

#ifndef HELPER_EXCEPTION_NODEBASEEXCEPTION_H_
#define HELPER_EXCEPTION_NODEBASEEXCEPTION_H_

#include <exception>
#include <string>

namespace helper {
namespace exception {

class NodeBaseException: public std::exception {
public:
	NodeBaseException(const std::string& msg) : message(msg) { }
	virtual ~NodeBaseException() { }

	virtual const char* what() const noexcept{
		return message.c_str();
	}
protected:
	const std::string& message;
};

}
} /* namespace helper */



#endif /* HELPER_EXCEPTION_NODEBASEEXCEPTION_H_ */
