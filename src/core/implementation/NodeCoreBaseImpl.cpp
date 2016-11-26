/*
 * NodeCoreBaseImpl.cpp
 *
 *  Created on: 26.11.2016
 *      Author: tobias
 */

#include "NodeCoreBaseImpl.h"

namespace core {
namespace implementation {

NodeCoreBaseImpl::NodeCoreBaseImpl(NodeCore* core)
: core(core), sendToAll(nullptr) {

}

void NodeCoreBaseImpl::process(const Message& message) {
	if (messages.insert(MessagePair(message.getNumber(), message)).second) {
		string content = to_string(core->getNodeInfo().NodeID);
		int number = message.getNumber();
		Message newMessage(MessageType::application, number, core->getNodeInfo().NodeID, content);
		(core->*sendToAll)(newMessage, message.getSourceID());
	}
}

} /* namespace implementation */
} /* namespace core */
