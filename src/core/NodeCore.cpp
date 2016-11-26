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

#include "../helper/interfaces/IConfigureNode.h"
#include "../network/NodeTransceiver.h"
#include "implementation/NodeCoreBaseImpl.h"


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

	if (content.find("Shutdown") != string::npos) {
		shutdown(message);
	} else if (content.find("Init") != string::npos) {
		//string c = trimPrefix(content, Message::TOKEN); // für das GERÜCHT
		string c = to_string(nodeInfo.NodeID);
		int number = message.getNumber();
		Message newMessage(MessageType::application, number, nodeInfo.NodeID, c);
		sendToDestinationsImpl(newMessage, neighbors);
	}
}

void NodeCore::handleApplicationMessage(const Message& message) {

}

bool NodeCore::sendToDestinationsImpl(const Message& message,
		const NodeMap& destinations) {

	bool successfully = false;

	for (NodeMap::const_iterator it = destinations.begin(); it != destinations.end(); ++it) {
		successfully = sendTo(message, it->second);
	}

	return successfully;
}

bool NodeCore::sendToDestinations(const Message& message, const int& expectedNodeID) {
	NodeMap otherNeighbors(neighbors.begin(), neighbors.end());
	otherNeighbors.erase(expectedNodeID);
	return sendToDestinationsImpl(message, otherNeighbors);
}

bool NodeCore::sendTo(const Message& message, const NodeInfo& destination) const {
	cout << getCurrentTime() <<" Send: " << message.toString() << " to Node " << destination.NodeID << endl;
	return transceiver->sendTo(destination, message.write());
}

// TODO: Fragen, ob Timestamp als Wert in Microsek OKAY ist
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

} /* namespace node */

