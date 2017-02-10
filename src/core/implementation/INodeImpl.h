/*
 * INodeImpl.h
 *
 *  Created on: 26.11.2016
 *      Author: tobias
 */

#ifndef CORE_IMPLEMENTATION_INODEIMPL_H_
#define CORE_IMPLEMENTATION_INODEIMPL_H_

#include <functional>
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
	virtual void process(Message* const message) = 0;

	virtual void getState(string& state) = 0;

	void setSendEcho(std::function<bool(const std::string& content)> sendEcho) {
		this->sendEcho = sendEcho;
	}
	void setSendTo(std::function<bool(Message*, const int&)> sendTo) {
		this->sendTo = sendTo;
	}

	void setSendToDestinations(std::function<bool(Message* const, const int&)> sendToAll) {
		this->sendToAll = sendToAll;
	}

	NodeCore* getCore() const {
		return core;
	}

	void setCore(NodeCore* core) {
		this->core = core;
	}

	virtual INodeImpl* prototype() = 0;

protected:
	NodeCore* core;
	//NodeCore::_sendEcho sendEcho;
	std::function<bool(const std::string& content)> sendEcho;

	//NodeCore::_sendTo sendTo;
	std::function<bool(Message*, const int&)> sendTo;

	//NodeCore::_sendToAll sendToAll;
	std::function<bool(Message* const, const int&)> sendToAll;
};

} /* namespace implementation */
} /* namespace core */

#endif /* CORE_IMPLEMENTATION_INODEIMPL_H_ */
