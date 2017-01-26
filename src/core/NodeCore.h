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
#include <vector>

#include "../helper/NodeInfo.h"
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
	typedef bool (NodeCore::*_sendEcho)(const string& content);
	typedef bool (NodeCore::*_sendTo)(Message* const, const int& nodeID);
	typedef bool (NodeCore::*_sendToAll)(Message* const, const int& excludedNodeID);
	typedef bool (NodeCore::*_sendResult)(Message* const message);

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

private:
	NodeInfo nodeInfo;
	NodeMap neighbors;
	bool isRunning;
	TransceiverBase* transceiver;
	INodeImpl* nodeImpl;
	vector<string*>* log;
	NodeInfo listenerNodeInfo;
	helper::time::VectorTime* vectorTime;

	EchoBuffer echoBuffer;

	void showDetails();

	void handleControlMessage(ControlMessage* const message);
	void handleApplicationMessage(ApplicationMessage* const message);
	void handleEchoMessage(Message* const message);
	bool sendEcho(const string& content);
	bool sendToDestinationsImpl(Message* const message, const NodeMap& destinations);
	bool sendToDestinations(Message* const message, const int& expectedNodeID);

	void shutdown(ControlMessage* const message);
	void sendSnapshot();
	bool sendToListener(Message* const message);
	bool sendStatusToListener(const string& status);
};

} /* namespace view */

#endif /* NODECORE_H_ */
