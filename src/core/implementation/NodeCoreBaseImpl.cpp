/*
 * NodeCoreBaseImpl.cpp
 *
 *  Created on: 26.11.2016
 *      Author: tobias
 */

#include "NodeCoreBaseImpl.h"
#include <iostream>

namespace core {
namespace implementation {

NodeCoreBaseImpl::NodeCoreBaseImpl() {
}

void NodeCoreBaseImpl::process(const Message& message) {
	// TODO Message aufbereiten

	//if (messageSet.insert(MinimalMessageInfoTupel(message.getSourceID(), message.getNumber())).second) {
	if (messages.insert(MessagePair(message.getNumber(), message)).second) {

		string content = to_string(core->getNodeInfo().NodeID);
		int number = message.getNumber();
		Message newMessage(MessageType::application, number, getCore()->getNodeInfo().NodeID, content);
		(getCore()->*sendToAll)(newMessage, message.getSourceID());
		messages.insert(MessagePair(number, newMessage));
	}
}

} /* namespace implementation */
} /* namespace core */
