/*
 * NodeCoreBaseImpl.h
 *
 *  Created on: 26.11.2016
 *      Author: tobias
 */

#ifndef CORE_IMPLEMENTATION_NODECOREBASEIMPL_H_
#define CORE_IMPLEMENTATION_NODECOREBASEIMPL_H_

#include "../../network/Message.h"
#include "INodeImpl.h"

namespace core {
namespace implementation {

typedef int MessageNumber;
typedef std::pair<MessageNumber, Message> MessagePair; // Number of Message and Message itself
typedef std::map<MessagePair::first_type, MessagePair::second_type> MessageMap;

class NodeCoreBaseImpl : public INodeImpl {
public:
	NodeCoreBaseImpl(NodeCore* core);
	virtual ~NodeCoreBaseImpl() { };
	virtual void process(const Message& message);

	virtual void setSendToDestinations(NodeCore::_sendToAll sendToAll) {
		this->sendToAll = sendToAll;
	}

private:
	NodeCore* core;
	NodeCore::_sendToAll sendToAll;
	MessageMap messages;
};

} /* namespace implementation */
} /* namespace core */

#endif /* CORE_IMPLEMENTATION_NODECOREBASEIMPL_H_ */
