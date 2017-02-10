/*
* ShuttingDownException.h
 *
 *  Created on: 05.02.2017
 *      Author: tobias
 */

#ifndef HELPER_EXCEPTION_SHUTTINGDOWNEXCEPTION_H_
#define HELPER_EXCEPTION_SHUTTINGDOWNEXCEPTION_H_

#include "NodeBaseException.h"

namespace helper {
namespace exception {

class ShuttingDownException: public NodeBaseException {
public:
	ShuttingDownException() : NodeBaseException("Node is shutting down!") { }
	virtual ~ShuttingDownException() { }
};

} /* namespace exception */
} /* namespace helper */

#endif /* HELPER_EXCEPTION_SHUTTINGDOWNEXCEPTION_H_ */
