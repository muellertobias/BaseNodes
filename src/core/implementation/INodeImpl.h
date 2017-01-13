/*
 * INodeImpl.h
 *
 *  Created on: 26.11.2016
 *      Author: tobias
 */

#ifndef CORE_IMPLEMENTATION_INODEIMPL_H_
#define CORE_IMPLEMENTATION_INODEIMPL_H_

#include "../../network/Message.h"
#include "../../NodeInfo.h"
#include "../NodeCore.h"
#include "../data/MessageMap.h"

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

protected:
	NodeCore::_sendToAll sendToAll;
};

} /* namespace implementation */
} /* namespace core */

#endif /* CORE_IMPLEMENTATION_INODEIMPL_H_ */
