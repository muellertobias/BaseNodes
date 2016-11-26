/*
 * NodeCore.h
 *
 *  Created on: 27.10.2016
 *      Author: tobias
 */

#ifndef NODECORE_H_
#define NODECORE_H_

#include <map>
#include <string>

#include "../network/Message.h"
#include "../NodeInfo.h"
#include "data/MessageMap.h"

namespace helper {
class ILogging;
namespace interfaces {
class IConfigureNode;
} /* namespace interfaces */
} /* namespace helper */
namespace network {
class TransceiverBase;
} /* namespace network */

using namespace std;
using namespace helper::interfaces;
using namespace network;
using namespace helper;

namespace node {

class NodeCore {
public:
	NodeCore(IConfigureNode* configurator);
	virtual ~NodeCore();

	void setNeighbors(NodeMap neighbors) {
		this->neighbors = neighbors;
	}

	const NodeMap& getNeighbors() const {
		return neighbors;
	}

	void loop();

	bool sendTo(const Message& message, const NodeInfo& destination) const;

private:
	NodeInfo nodeInfo;
	NodeMap neighbors;
	MessageMap messages;
	bool isRunning;
	TransceiverBase* transceiver;

	void showDetails();
	Message receive() const;
	void handleControlMessage(const Message& message);
	void handleApplicationMessage(const Message& message);
	bool sendToDestinations(const Message& message, const NodeMap& destinations);
	bool sendToDestinations(const Message& message, const NodeMap& destinations, const int& expectedNodeID);
	std::string getCurrentTime() const;

	void shutdown(const Message& message);
};

} /* namespace view */

#endif /* NODECORE_H_ */
