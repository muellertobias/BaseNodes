/*
 * NodeCore.cpp
 *
 *  Created on: 27.10.2016
 *      Author: tobias
 */

#include "NodeCore.h"

#include <stddef.h>
#include <iterator>
#include <sstream>

#include "../helper/Constants.h"
#include "../helper/exception/NetworkException.h"
#include "../helper/exception/NodeBaseException.h"
#include "../helper/settings/Settings.h"
#include "../helper/utilities/tinyxml2.h"
#include "../network/NodeTransceiver.h"
#include "implementation/INodeImpl.h"
#include "implementation/NodeCoreBaseImpl.h"


namespace core {

NodeCore::NodeCore(Settings* settings) {
	this->nodeInfo = settings->getCurrentNodeInfo();

	// Communication
	this->neighbors = settings->getNeighbors();
	this->listenerNodeInfo = settings->getNodeInfo(ListenerNodeID);
	this->transceiver = settings->getTransceiver();
	isRunning = true;

	// Implementation
	this->nodeImpl = settings->getNodeImplementation();
	this->nodeImpl->setCore(this);
	this->nodeImpl->setSendToDestinations(&NodeCore::sendToDestinations);
	this->nodeImpl->setSendResult(&NodeCore::sendToListener);

	// Simple Logging
	this->log = new vector<string*>();

	// Vectortime
	vectorTime = new VectorTime(this->nodeInfo.NodeID);

	// Echo
	this->echoData = { 0, 0, 0};
}

NodeCore::~NodeCore() {
	delete transceiver;

	for (size_t i = 0; i < log->size(); ++i) {
		log[i].clear();
	}

	log->clear();
	delete log;

	delete nodeImpl;
}

void NodeCore::loop() {
	showDetails();

	while (isRunning) {
		try {
			const Message& message = receive();

			switch (message.getType()) {
			case MessageType::control:
				handleControlMessage(message);
				break;
			case MessageType::application:
				handleApplicationMessage(message);
				break;
			case MessageType::explorer:
			case MessageType::echo:
				handleEchoMessage(message);
				break;
			default:
				cerr << "Unexpected type of message!" << endl;
				break;
			}
		} catch (const helper::exception::NetworkException& ex) {
			cerr << ex.what() << endl;
		}
	}
	transceiver->closeReceiver();
}

void NodeCore::showDetails() {
	stringstream strStream;
	string address = transceiver->resolve(this->nodeInfo);;

	strStream << "NodeID: " << nodeInfo.NodeID << "\t" <<
			"Address: " << address << "\n" << endl;

	for (NodeMap::iterator i = neighbors.begin(); i != neighbors.end(); ++i) {
		strStream << "Neighbor: " << i->first << endl;
	}
	sendStatusToListener(strStream.str());
}

const Message NodeCore::receive() {
	string incomingStr = transceiver->receive();
	Message msg(incomingStr);
	if (msg.getType() == MessageType::application) {
		this->vectorTime->increase();
		this->vectorTime->merge(msg.getVectorTimes());
		this->vectorTime->setTime(this->nodeInfo.NodeID, this->vectorTime->getMaximum());
		cout << msg.toString() << "/" << nodeInfo.NodeID << "-Local: "<< this->vectorTime->getLocalTime() << std::endl;
	}

	string* logStr = new string(to_string(this->vectorTime->getLocalTime()) + " Receive: " + msg.toString());
	log->push_back(logStr);

	return msg;
}

void NodeCore::handleControlMessage(const Message& message) {
	const string& content = message.getContent();
	if (content.find(constants::ShutdownMessage) != string::npos) {
		shutdown(message);
	} else if (content.find("Snapshot") != string::npos) {
		sendSnapshot();
	} else if (content.find("Echo") != string::npos) {
		this->echoData.EchoID = message.getNumber();
		this->echoData.FirstNeighborID = nodeInfo.NodeID;
		this->echoData.counter = 0;
		Message explorerMsg(MessageType::explorer, message.getNumber(), nodeInfo.NodeID, "Explorer");
		sendToDestinationsImpl(explorerMsg, neighbors);

	} else {
		this->nodeImpl->process(message);
	}
}

void NodeCore::handleApplicationMessage(const Message& message) {
	nodeImpl->process(message);
}

void NodeCore::handleEchoMessage(const Message& message) {
	if (message.getType() == MessageType::explorer) {
		if (this->echoData.FirstNeighborID != message.getSourceID() && this->echoData.EchoID != message.getNumber()) {
			this->echoData.EchoID = message.getNumber();
			this->echoData.FirstNeighborID = message.getSourceID();
			this->echoData.counter = 1;

			nodeImpl->process(message);

			Message explorerMsg(MessageType::explorer, message.getNumber(), nodeInfo.NodeID, message.getContent());
			sendToDestinations(explorerMsg, message.getSourceID());
			cout << nodeInfo.NodeID << " - Send Explorer!" << endl;

		} else if (this->echoData.FirstNeighborID != message.getSourceID() && this->echoData.EchoID == message.getNumber()) {
			this->echoData.counter++;
		} else {
			// FEHLER?! Zweimal Explorer vom selben Knoten mit selber ECHO-ID (=MessageNumber)
		}
	} else if (message.getType() == MessageType::echo) {
		this->echoData.counter++;
	}

	if (this->echoData.counter == this->neighbors.size()) {
		if (this->echoData.FirstNeighborID == nodeInfo.NodeID) {
			//Echo erfolgreich!
			cout << nodeInfo.NodeID << " - ECHO erfolgreich!" << endl;
		} else {
			cout << nodeInfo.NodeID << " - Send Echo!" << endl;
			Message echoMsg(MessageType::echo, message.getNumber(), nodeInfo.NodeID, "Echo");
			sendTo(echoMsg, this->neighbors.at(echoData.FirstNeighborID));
		}
	}
}

bool NodeCore::sendToDestinationsImpl(const Message& message, const NodeMap& destinations) {
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
	if (message.getType() == MessageType::application) {
		this->vectorTime->increase();
	}
	Message msg = const_cast<Message&>(message);
	msg.setDestinationId(destination.NodeID);

	if (this->nodeInfo.NodeID > 0)
		msg.setVectorTimes(this->vectorTime->getTimeMap());

	if (message.getType() != MessageType::log)
		log->push_back(new string(to_string(vectorTime->getLocalTime()) + " Send to "  + to_string(destination.NodeID) + ": " + msg.toString()));

	return transceiver->sendTo(destination, msg.write());
}

void NodeCore::shutdown(const Message& message) {
	if (message.getSourceID() == this->nodeInfo.NodeID || message.getSourceID() == -1) {
		isRunning = !isRunning;
		sendSnapshot();
	} else {
		NodeMap::iterator node = neighbors.find(message.getSourceID());
		if (node != neighbors.end()) {
			sendToDestinationsImpl(message, neighbors);
		} else {
			sendTo(message, node->second);
		}
	}

	if (message.getSourceID() == -1) {
		sendToDestinationsImpl(message, neighbors);
	}
}

void NodeCore::sendSnapshot() {
	using namespace tinyxml2;

	XMLDocument doc;
	XMLElement* root = doc.NewElement("log");
	int length = log->size();
	root->QueryIntAttribute("length", &length);

	for (vector<string*>::const_iterator it = log->begin(); it != log->end(); ++it) {
		string* entry = (*it);
		XMLElement* element = doc.NewElement("logEntry");
		element->SetText(entry->c_str());
		root->InsertEndChild(element);
	}

	doc.InsertEndChild(root);

	XMLPrinter p;
	doc.Print(&p);

	Message msg(MessageType::log, 0, nodeInfo.NodeID, string(p.CStr()));
	sendToListener(msg);

	//cout << msg.toString() << endl;
	doc.Clear();
}

bool NodeCore::sendToListener(const Message& message) {
	Message msg = const_cast<Message&>(message);
	msg.setVectorTimes(this->vectorTime->getTimeMap());
	return sendTo(msg, listenerNodeInfo);
}

bool NodeCore::sendStatusToListener(const string& status) {
	Message statusMsg(MessageType::control, 0, nodeInfo.NodeID, status);
	return sendToListener(statusMsg);
}


} /* namespace node */

