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

NodeCore::NodeCore(Settings* setting) {
	bool isReceiver = true;
	if (setting->getNodeId() > 0) {
		this->nodeInfo = setting->getCurrentNodeInfo();
	} else {
		isReceiver = false;
	}
	this->neighbors = setting->getNeighbors();
	this->listenerNodeInfo = setting->getNodeInfo(ListenerNodeID);
	this->transceiver = new NodeTransceiver(this->nodeInfo, 10, isReceiver);
	isRunning = true;

	// Implementierung hier austauschbar
	this->nodeImpl = setting->getNodeImplementation();
	this->nodeImpl->setCore(this);
	this->nodeImpl->setSendToDestinations(&NodeCore::sendToDestinations);
	this->nodeImpl->setSendResult(&NodeCore::sendToListener);

	this->log = new vector<string*>();
	vectorTime = new VectorTime(this->nodeInfo.NodeID);
}

NodeCore::~NodeCore() {
	delete transceiver;

	for (size_t i = 0; i < log->size(); ++i) {
		log[i].clear();
		//delete log[i];
	}

	log->clear();
	delete log;

	delete nodeImpl;
}

void NodeCore::loop() {
	showDetails();

	while (isRunning) {
		try {
			//cout << "Listen..." << endl;
			const Message& message = receive();

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
	//cout << "Shutdown..." << endl;
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
	} else {
		this->nodeImpl->process(message);
		//string c = to_string(nodeInfo.NodeID);
//		int number = message.getNumber();
//		Message newMessage(MessageType::application, number, nodeInfo.NodeID, message.getContent());
//		sendToDestinationsImpl(newMessage, neighbors);
	}
}

void NodeCore::handleApplicationMessage(const Message& message) {
	nodeImpl->process(message);
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
//	NodeInfo info;
//	info.NodeID = 0;
//	inet_pton(AF_INET, "127.0.0.1", &(info.Address.sin_addr));
//	info.Address.sin_port = 5000;
//	info.Address.sin_family = AF_INET;

	Message msg = const_cast<Message&>(message);
	msg.setVectorTimes(this->vectorTime->getTimeMap());
	return sendTo(msg, listenerNodeInfo);
}

bool NodeCore::sendStatusToListener(const string& status) {
	Message statusMsg(MessageType::control, 0, nodeInfo.NodeID, status);
	return sendToListener(statusMsg);
}


} /* namespace node */

