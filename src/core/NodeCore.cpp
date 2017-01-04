/*
 * NodeCore.cpp
 *
 *  Created on: 27.10.2016
 *      Author: tobias
 */

#include "NodeCore.h"

#include <chrono>
#include <iostream>
#include <sstream>
#include <utility>


#include "../helper/exception/NetworkException.h"
#include "../helper/interfaces/IConfigureNode.h"
#include "../network/NodeTransceiver.h"
#include "implementation/NodeCoreBaseImpl.h"
#include "../Constants.h"


namespace core {

NodeCore::NodeCore(IConfigureNode* configurator) {
	this->nodeInfo = configurator->getCurrentNodeInfo();
	this->neighbors = configurator->getNeighbors();
	this->transceiver = new NodeTransceiver(this->nodeInfo, 10);
	isRunning = true;
	this->nodeImpl = new NodeCoreBaseImpl(this);
	this->nodeImpl->setSendToDestinations(&NodeCore::sendToDestinations);
}

NodeCore::~NodeCore() {
}

void NodeCore::loop() {
	showDetails();

	while (isRunning) {
		try {
			cout << "Listen..." << endl;
			const Message& message = receive();
			cout << getCurrentTime() << " Receive: " << message.toString() << endl;

			if (message.getType() == MessageType::control) {
				handleControlMessage(message);
			} else if (message.getType() == MessageType::application) {
				handleApplicationMessage(message);
			} else {
				cerr << "Unexpected type of message!" << endl;
			}

		} catch (const helper::exception::NetworkException& ex) {
			cerr << ex.what() << endl;
		}
	}
	cout << "Shutdown..." << endl;
	transceiver->closeReceiver();
}

void NodeCore::showDetails() {

	string address = transceiver->resolve(this->nodeInfo);;

	cout << "NodeID: " << nodeInfo.NodeID << "\t" <<
			"Address: " << address << "\n" << endl;

	for (NodeMap::iterator i = neighbors.begin(); i != neighbors.end(); ++i) {
		cout << "Neighbor: " << i->first << endl;
	}
}

Message NodeCore::receive() const {
	string incomingStr = transceiver->receive();
	return Message(incomingStr);
}

void NodeCore::handleControlMessage(const Message& message) {
	const string& content = message.getContent();
	if (content.find(constants::ShutdownMessage) != string::npos) {
		shutdown(message);
	} else if (content.find("Snapshot") != string::npos) {
		sendSnapshot();
	} else {
		string c = to_string(nodeInfo.NodeID);
		int number = message.getNumber();
		Message newMessage(MessageType::application, number, nodeInfo.NodeID, c);
		sendToDestinationsImpl(newMessage, neighbors);
	}
}

void NodeCore::handleApplicationMessage(const Message& message) {
	nodeImpl->process(message);
}

bool NodeCore::sendToDestinationsImpl(const Message& message,
		const NodeMap& destinations) {

	bool successfully = false;

	for (NodeMap::const_iterator it = destinations.begin(); it != destinations.end(); ++it) {
		successfully = sendTo(message, it->second);
	}

	return successfully;
}

bool NodeCore::sendToDestinations(const Message& message, const int& excludedNodeID) {
	NodeMap otherNeighbors(neighbors.begin(), neighbors.end());
	otherNeighbors.erase(excludedNodeID);
	return sendToDestinationsImpl(message, otherNeighbors);
}

bool NodeCore::sendTo(const Message& message, const NodeInfo& destination) const {
	cout << getCurrentTime() <<" Send: " << message.toString() << " to Node " << destination.NodeID << endl;
	return transceiver->sendTo(destination, message.write()); // native (true) or XML (false)
}

// TODO: Überlegen, wegen Einführung Vectorzeit -> Übung 2
std::string NodeCore::getCurrentTime() const {
	auto now = chrono::high_resolution_clock::now();
	ostringstream oss;
	auto d = chrono::duration_cast<chrono::microseconds>(now.time_since_epoch());
	oss << d.count();
	return oss.str();
}

void NodeCore::shutdown(const Message& message) {
	if (message.getSourceID() == this->nodeInfo.NodeID) {
		isRunning = !isRunning;
	} else if (message.getSourceID() == -1) {
		isRunning = !isRunning;
		sendToDestinationsImpl(message, neighbors);
	} else {
		NodeMap::iterator node = neighbors.find(message.getSourceID());
		if (node != neighbors.end()) {
			sendToDestinationsImpl(message, neighbors);
		} else {
			sendTo(message, node->second);
		}
	}
}

void NodeCore::sendSnapshot() {
	cout << "SNAP" << endl;
	string hallo = "Hallo, ich bin " + nodeInfo.NodeID;

	Message msg(MessageType::control, 0, 0, hallo);

	NodeInfo info;
	info.NodeID = 0;

	inet_pton(AF_INET, "127.0.0.1", &(info.Address.sin_addr));
	info.Address.sin_port = 4999;
	info.Address.sin_family = AF_INET;
	sendTo(msg, info);
}

} /* namespace node */

