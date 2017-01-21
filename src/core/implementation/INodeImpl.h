/*
 * INodeImpl.h
 *
 *  Created on: 26.11.2016
 *      Author: tobias
 */

#ifndef CORE_IMPLEMENTATION_INODEIMPL_H_
#define CORE_IMPLEMENTATION_INODEIMPL_H_

#include "../../message/Message.h"
#include "../NodeCore.h"

namespace core {
namespace implementation {

using namespace network;
using namespace std;


class INodeImpl {
public:
	virtual ~INodeImpl() {};
	virtual void process(const Message& message) = 0;

	void setSendToDestinations(NodeCore::_sendToAll sendToAll) {
		this->sendToAll = sendToAll;
	}

	void setSendResult(NodeCore::_sendResult sendResult) {
		this->sendResultImpl = sendResult;
	}

	NodeCore* getCore() const {
		return core;
	}

	void setCore(NodeCore* core) {
		this->core = core;
	}

protected:
	NodeCore* core;
	NodeCore::_sendToAll sendToAll;
	NodeCore::_sendResult sendResultImpl;
};

} /* namespace implementation */
} /* namespace core */

#endif /* CORE_IMPLEMENTATION_INODEIMPL_H_ */
