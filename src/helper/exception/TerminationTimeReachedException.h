/*
 * TerminationTimeReachedException.h
 *
 *  Created on: 26.01.2017
 *      Author: tobias
 */

#ifndef HELPER_EXCEPTION_TERMINATIONTIMEREACHEDEXCEPTION_H_
#define HELPER_EXCEPTION_TERMINATIONTIMEREACHEDEXCEPTION_H_

#include "NodeBaseException.h"

namespace helper {
namespace exception {

class TerminationTimeReachedException: public NodeBaseException {
public:
	TerminationTimeReachedException();
	virtual ~TerminationTimeReachedException();
};

} /* namespace exception */
} /* namespace helper */

#endif /* HELPER_EXCEPTION_TERMINATIONTIMEREACHEDEXCEPTION_H_ */
