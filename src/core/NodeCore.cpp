/*
 * NodeCore.cpp
 *
 *  Created on: 27.10.2016
 *      Author: tobias
 */

#include "NodeCore.h"

#include <stddef.h>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <sstream>
#include <utility>

#include "../helper/Constants.h"
#include "../helper/exception/NetworkException.h"
#include "../helper/randomizer/Random.h"
#include "../helper/settings/Settings.h"
#include "../helper/utilities/tinyxml2.h"
#include "../helper/utilities/utilities.h"
#include "../network/TransceiverBase.h"
#include "implementation/INodeImpl.h"


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
	this->nodeImpl->setSendEcho(&NodeCore::sendEcho);
	bool (NodeCore::*temp)(const Message&, const int&) const = &NodeCore::sendTo;
	this->nodeImpl->setSendTo(temp);
	this->nodeImpl->setSendToDestinations(&NodeCore::sendToDestinations);
	this->nodeImpl->setSendResult(&NodeCore::sendToListener);

	// Simple Logging
	this->log = new vector<string*>();

	// Vectortime
	vectorTime = new VectorTime(this->nodeInfo.NodeID);

	// Echo
	//this->echoData = { 0, 0, 0};
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

	try {
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
				helper::utilities::writeLog(__FUNCTION__, ex);
			}
		}
	} catch (std::exception& e) {
		helper::utilities::writeLog(__FUNCTION__, e);
	}

	transceiver->closeReceiver();
}

void NodeCore::showDetails() {
	stringstream strStream;
	string address;
	transceiver->resolve(this->nodeInfo, address);

	strStream << "NodeID: " << nodeInfo.NodeID << "\t" <<
			"Address: " << address << "\n" << endl;

	for (NodeMap::iterator i = neighbors.begin(); i != neighbors.end(); ++i) {
		strStream << "Neighbor: " << i->first << endl;
	}
	sendStatusToListener(strStream.str());
}

const Message NodeCore::receive() {
	try {
		string incomingStr = transceiver->receive();

		Message msg(incomingStr);
		if (msg.getType() != MessageType::control) {
			this->vectorTime->increase();
			this->vectorTime->merge(msg.getVectorTimes());
			this->vectorTime->setTime(this->nodeInfo.NodeID, this->vectorTime->getMaximum());
			//cout << msg.toString() << "/" << nodeInfo.NodeID << "-Local: "<< this->vectorTime->getLocalTime() << std::endl;
		}

		//string* logStr = new string(to_string(this->vectorTime->getLocalTime()) + " Receive: " + msg.toString());
		//log->push_back(logStr);

		return msg;
	} catch (std::exception& e) {
		helper::utilities::writeLog(__FUNCTION__, e);
		throw e;
	}
}

void NodeCore::handleControlMessage(const Message& message) {
	try {
		const string& content = message.getContent();
		if (content == constants::ShutdownMessage) {
			shutdown(message);
		} else if (content == "Snapshot") {
			sendSnapshot();
		} else if (content == "Echo") {
			Echos echo;
			echo.EchoID = message.getNumber();
			echo.FirstNeighborID = nodeInfo.NodeID;
			echo.counter = 0;
			this->echoBuffer.insert(EchoEntry(echo.EchoID, echo));

			Message explorerMsg(MessageType::explorer, message.getNumber(), nodeInfo.NodeID, "Explorer");
			sendToDestinationsImpl(explorerMsg, neighbors);
		} else if (content == "Reset") {
			// Resette Implementation und Vectorzeit
		} else {
			this->nodeImpl->process(message);
		}
	} catch (std::exception& e) {
		helper::utilities::writeLog(__FUNCTION__, e);
		throw e;
	}
}

void NodeCore::handleApplicationMessage(const Message& message) {
	try {
		nodeImpl->process(message);
	} catch (std::exception& e) {
		helper::utilities::writeLog(__FUNCTION__, e);
		throw e;
	}
}

void NodeCore::handleEchoMessage(const Message& message) {
	try {
		EchoBuffer::iterator it = this->echoBuffer.find(message.getNumber());

		if (message.getType() == MessageType::explorer) {
			if (it == this->echoBuffer.end()) {
				Echos echo;
				echo.EchoID = message.getNumber();
				echo.FirstNeighborID = message.getSourceID();
				echo.counter = 1;
				this->echoBuffer.insert(EchoEntry(echo.EchoID, echo));

				nodeImpl->process(message);

				Message explorerMsg(MessageType::explorer, message.getNumber(), nodeInfo.NodeID, message.getContent());
				sendToDestinations(explorerMsg, message.getSourceID());
				//cout << nodeInfo.NodeID << " - Send Explorer!" << endl;
			} else {
				it->second.counter++;

				//if (it->second.FirstNeighborID != message.getSourceID()) {
					// FEHLER?! Zweimal Explorer vom selben Knoten mit selber ECHO-ID (=MessageNumber)
				//}
			}
		} else if (message.getType() == MessageType::echo) {
			it->second.counter++;
			//cout << nodeInfo.NodeID << " - Received Echo!" << endl;
		}

		if (it->second.counter == this->neighbors.size()) {
			if (it->second.FirstNeighborID == nodeInfo.NodeID) {
				//Echo erfolgreich!
				//cout << nodeInfo.NodeID << " - ECHO erfolgreich!" << endl;
				this->nodeImpl->process(message);
			} else {
				//cout << nodeInfo.NodeID << " - Send Echo!" << endl;
				Message echoMsg(MessageType::echo, message.getNumber(), nodeInfo.NodeID, message.getContent());
				sendTo(echoMsg, this->neighbors.at(it->second.FirstNeighborID));
			}
		}
	} catch (std::exception& e) {
		helper::utilities::writeLog(__FUNCTION__, e);
		throw e;
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
	if (excludedNodeID != 0) {
		NodeMap otherNeighbors(neighbors.begin(), neighbors.end());
		otherNeighbors.erase(excludedNodeID);
		return sendToDestinationsImpl(message, otherNeighbors);
	} else {
		return sendToDestinationsImpl(message, neighbors);
	}
}

bool NodeCore::sendEcho(const string& content) {
	Echos echo;
	echo.EchoID = randomizer::random(0, 9999);
	echo.FirstNeighborID =  nodeInfo.NodeID;
	echo.counter = 0;
	this->echoBuffer.insert(EchoEntry(echo.EchoID, echo));
	Message explorerMsg(MessageType::explorer, echo.EchoID, nodeInfo.NodeID, content);
	return this->sendToDestinationsImpl(explorerMsg, neighbors);
}

bool NodeCore::sendTo(const Message& message, const int& nodeID) const {
	NodeInfo destination;
	this->transceiver->resolve(nodeID, destination);
	return this->sendTo(message, destination);
}

bool NodeCore::sendTo(const Message& message, const NodeInfo& destination) const {
	if (message.getType() == MessageType::application) {
		this->vectorTime->increase();
	}
	Message msg = const_cast<Message&>(message);
	msg.setDestinationId(destination.NodeID);

	if (msg.getSourceID() != -1)
		msg.setSourceID(nodeInfo.NodeID);

	if (this->nodeInfo.NodeID > 0)
		msg.setVectorTimes(this->vectorTime->getTimeMap());

//	if (message.getType() != MessageType::log)
//		log->push_back(new string(to_string(vectorTime->getLocalTime()) + " Send to "  + to_string(destination.NodeID) + ": " + msg.toString()));

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

