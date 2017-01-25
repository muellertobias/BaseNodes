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
#include <string>

namespace core {
namespace implementation {

using namespace message;
using namespace std;


class INodeImpl {
public:
	virtual ~INodeImpl() {};
	virtual void process(const Message& message) = 0;
	virtual void getState(string& state) = 0;

	void setSendEcho(NodeCore::_sendEcho sendEcho) {
		this->sendEcho = sendEcho;
	}
	void setSendTo(NodeCore::_sendTo sendTo) {
		this->sendTo = sendTo;
	}

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
	NodeCore::_sendEcho sendEcho;
	NodeCore::_sendTo sendTo;
	NodeCore::_sendToAll sendToAll;
	NodeCore::_sendResult sendResultImpl;
};

} /* namespace implementation */
} /* namespace core */

#endif /* CORE_IMPLEMENTATION_INODEIMPL_H_ */
