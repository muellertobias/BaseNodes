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

#include "../helper/NodeInfo.h"
#include "../helper/time/VectorTime.h"
#include "data/Echo.h"

namespace helper {
namespace time {
class VectorTime;
} /* namespace time */
} /* namespace helper */
namespace message {
class ApplicationMessage;
class ControlMessage;
class Message;
} /* namespace message */

namespace core {
namespace implementation {
class INodeImpl;
} /* namespace implementation */
} /* namespace core */

namespace helper {
class ILogging;
namespace settings {
class Settings;
} /* namespace settings */
} /* namespace helper */
namespace network {
class TransceiverBase;
} /* namespace network */

using namespace std;
using namespace helper::settings;
using namespace network;
using namespace helper;
using namespace message;

namespace core {
using namespace implementation;

class NodeCore {
public:
	NodeCore(Settings* configurator);
	virtual ~NodeCore();

	void setNeighbors(NodeMap neighbors) {
		this->neighbors = neighbors;
	}

	const NodeMap& getNeighbors() const {
		return neighbors;
	}

	void loop();

	bool sendTo(Message* const message, const int& nodeID);
	bool sendTo(Message* const message, const NodeInfo& destination);

	const NodeInfo& getNodeInfo() const {
		return nodeInfo;
	}

	Message* receive();

	int getLocalTime() const {
		return this->vectorTime->getLocalTime();
	}

private:
	NodeInfo nodeInfo;
	NodeMap neighbors;
	bool isRunning;
	bool isShuttingDown;
	bool isMarked;
	TransceiverBase* transceiver;
	INodeImpl* nodeImpl;
	NodeInfo listenerNodeInfo;
	helper::time::VectorTime* vectorTime;
	EchoBuffer echoBuffer;
	INodeImpl* pastImpl;

	void showDetails();

	void handleControlMessage(ControlMessage* const message);
	void handleApplicationMessage(ApplicationMessage* const message);
	void handleEchoMessage(Message* const message);
	bool sendEcho(const string& content);
	bool sendToDestinationsImpl(Message* const message, const NodeMap& destinations);
	bool sendToDestinations(Message* const message, const int& expectedNodeID);

	bool sendStatusToListener(const string& status);

	void createTemporaryPastImplementation();
	void processImplementations(Message* const message);
	void initilizeControlEchoMessage(const int& echoID, const string& content);
	void processFirstEchoMessage(Message* const message);
};

} /* namespace view */

#endif /* NODECORE_H_ */
