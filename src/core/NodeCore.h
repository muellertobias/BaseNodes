/*
 * NodeCore.h
 *
 *  Created on: 27.10.2016
 *      Author: tobias
 */

#ifndef NODECORE_H_
#define NODECORE_H_

#include <map>
#include <vector>
#include <string>

#include "../helper/NodeInfo.h"
#include "../helper/time/VectorTime.h"
#include "../message/Message.h"
#include "data/Echo.h"

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
	typedef bool (NodeCore::*_sendTo)(const Message&, const int& nodeID) const;
	typedef bool (NodeCore::*_sendToAll)(const Message&, const int& excludedNodeID);
	typedef bool (NodeCore::*_sendResult)(const Message&);

	NodeCore(Settings* configurator);
	virtual ~NodeCore();

	void setNeighbors(NodeMap neighbors) {
		this->neighbors = neighbors;
	}

	const NodeMap& getNeighbors() const {
		return neighbors;
	}

	void loop();

	bool sendTo(const Message& message, const int& nodeID) const;
	bool sendTo(const Message& message, const NodeInfo& destination) const;

	const NodeInfo& getNodeInfo() const {
		return nodeInfo;
	}

	const Message receive();

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

	void handleControlMessage(const Message& message);
	void handleApplicationMessage(const Message& message);
	void handleEchoMessage(const Message& message);
	bool sendEcho(const string& content);
	bool sendToDestinationsImpl(const Message& message, const NodeMap& destinations);
	bool sendToDestinations(const Message& message, const int& expectedNodeID);

	void shutdown(const Message& message);
	void sendSnapshot();
	bool sendToListener(const Message& message);
	bool sendStatusToListener(const string& status);
};

} /* namespace view */

#endif /* NODECORE_H_ */
