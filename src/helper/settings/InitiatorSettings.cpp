/*
 * InitiatorSettings.cpp
 *
 *  Created on: 25.01.2017
 *      Author: tobias
 */

#include "InitiatorSettings.h"
#include "../../network/NodeTransceiver.h"

namespace helper {
namespace settings {

InitiatorSettings::InitiatorSettings(const string& filename)
	: NodeBaseSettings(filename) {
}

InitiatorSettings::~InitiatorSettings() {
	// TODO Auto-generated destructor stub
}

network::TransceiverBase* InitiatorSettings::getTransceiver() {
	bool isReceiver = this->nodeID < 0 ? false : true;
	return new network::NodeTransceiver(this->getCurrentNodeInfo(), numberOfConnections, this->allNodes, isReceiver);
}

} /* namespace settings */
} /* namespace helper */
