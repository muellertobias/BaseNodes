/*
 * RumorNodeCoreImpl.cpp
 *
 *  Created on: 08.01.2017
 *      Author: tobias
 */

#include "RumorNodeCoreImpl.h"

namespace core {
namespace implementation {
namespace rumor {


RumorNodeCoreImpl::RumorNodeCoreImpl(int threshold) : threshold(threshold) {
}

RumorNodeCoreImpl::~RumorNodeCoreImpl() {
	// TODO delete
}

void RumorNodeCoreImpl::process(const Message& message) {

	RumorInfoMap::iterator it = rumors.find(message.getContent());

	if (it != rumors.end()) { // Gerücht existiert, wird hochgezählt und SourceID wird gespeichert. Das Gerücht wird nicht mehr verteilt.
		it->second->counter++;

	} else { // Kennt Gerücht nicht: Lege Gerücht an, verteile es an alle außer SourceID
		RumorInfo* rumor = new RumorInfo();
		rumor->Rumor = message.getContent();
		rumor->Sources.push_back(message.getSourceID());
		rumor->counter = 1;

		rumors.insert(RumorInfoPair(rumor->Rumor, rumor));

		Message newMessage(MessageType::application, message.getNumber(), getCore()->getNodeInfo().NodeID, rumor->Rumor);

		(getCore()->*sendToAll)(newMessage, message.getSourceID());
	}
}

} /* namespace rumor */
} /* namespace implementation */
} /* namespace core */

