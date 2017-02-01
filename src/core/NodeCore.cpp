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
#include "../helper/exception/TerminationTimeReachedException.h"
#include "../helper/randomizer/Random.h"
#include "../helper/settings/Settings.h"
#include "../helper/time/VectorTime.h"
#include "../helper/utilities/tinyxml2.h"
#include "../helper/utilities/utilities.h"
#include "../message/ApplicationMessage.h"
#include "../message/ControlMessage.h"
#include "../message/Message.h"
#include "../message/MessageFactory.h"
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
	bool (NodeCore::*temp)(Message*, const int&) = &NodeCore::sendTo;
	this->nodeImpl->setSendTo(temp);
	this->nodeImpl->setSendToDestinations(&NodeCore::sendToDestinations);
	this->nodeImpl->setSendResult(&NodeCore::sendToListener);

	// Simple Logging
	this->log = new vector<string*>();

	// Vectortime
	vectorTime = new VectorTime(this->nodeInfo.NodeID);
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
				Message* message = receive();

				if (message->getType() == MessageSubType::echo || message->getType() == MessageSubType::explorer) {
					handleEchoMessage(message);
				} else if (dynamic_cast<ControlMessage*>(message) != NULL) {
					handleControlMessage((ControlMessage*)message);
				} else if (dynamic_cast<ApplicationMessage*>(message) != NULL) {
					handleApplicationMessage((ApplicationMessage*)message);
				}
			} catch (const helper::exception::NetworkException& ex) {
				helper::utilities::writeLog(__FUNCTION__, ex);
			} catch (std::exception& e) {
				throw e;
			}
		}
	} catch (const helper::exception::TerminationTimeReachedException&) {
		// Terminierung
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

Message* NodeCore::receive() {
	try {
		string incomingStr = transceiver->receive();

		Message* msg = MessageFactory::create(incomingStr);

		if (dynamic_cast<ApplicationMessage*>(msg) != NULL) {
			this->vectorTime->increase();
			this->vectorTime->merge(msg->getVectorTimes());
			this->vectorTime->setTime(this->nodeInfo.NodeID, this->vectorTime->getMaximum());
			if (this->vectorTime->isTerminated()) {
				throw helper::exception::TerminationTimeReachedException();
			}
			//cout << nodeInfo.NodeID << " - Time: " << this->vectorTime->getLocalTime() << endl;
		}


		//string* logStr = new string(to_string(this->vectorTime->getLocalTime()) + " Receive: " + msg.toString());
		//log->push_back(logStr);

		return msg;
	} catch (std::exception& e) {
		helper::utilities::writeLog(__FUNCTION__, e);
		throw e;
	}
}

void NodeCore::handleControlMessage(ControlMessage* const message) {
	try {
		const string& content = message->getContent();
		//cout << content << endl;
		if (content == constants::SHUTDOWN) {
			isRunning = !isRunning;
		} else if (content == constants::SHUTDOWN_ALL) {
			Message* shutdownMsg = new ControlMessage(MessageSubType::normal, message->getNumber(), nodeInfo.NodeID, constants::SHUTDOWN_ALL);
			this->sendToDestinationsImpl(shutdownMsg, neighbors);
			isRunning = !isRunning;
		} else if (content == constants::SNAPSHOT) {
			sendSnapshot();
		} else if (content == constants::SHUTDOWN_ECHO) {
			Echos echo;
			echo.EchoID = message->getNumber();
			echo.FirstNeighborID = nodeInfo.NodeID;
			echo.counter = 0;
			this->echoBuffer.insert(EchoEntry(echo.EchoID, echo));
			Message* explorerMsg = new ControlMessage(MessageSubType::explorer, message->getNumber(), nodeInfo.NodeID, constants::SHUTDOWN_ECHO);
			sendToDestinationsImpl(explorerMsg, neighbors);
		} else if (content == constants::RESET) {
			// Resette Implementation und Vectorzeit
		} else if (message->getType() == MessageSubType::parametrize){
			if (helper::utilities::isNumber(message->getContent())) {
				//this->vectorTime->setTermininationTime(stoi(message->getContent()));
				// TODO noch die terminierungszeit überwachen!
				Echos echo;
				echo.EchoID = message->getNumber();
				echo.FirstNeighborID = nodeInfo.NodeID;
				echo.counter = 0;
				this->echoBuffer.insert(EchoEntry(echo.EchoID, echo));
				Message* explorerMsg = new ControlMessage(MessageSubType::explorer, message->getNumber(), nodeInfo.NodeID, message->getContent());
				sendToDestinationsImpl(explorerMsg, neighbors);
			}
		} else {
			this->nodeImpl->process(message);
		}
	} catch (std::exception& e) {
		helper::utilities::writeLog(__FUNCTION__, e);
		throw e;
	}
}

void NodeCore::handleApplicationMessage(ApplicationMessage* const message) {
	try {
		nodeImpl->process(message);
	} catch (std::exception& e) {
		helper::utilities::writeLog(__FUNCTION__, e);
		throw e;
	}
}

void NodeCore::handleEchoMessage(Message* const message) {
	try {
		EchoBuffer::iterator it = this->echoBuffer.find(message->getNumber());

		if (message->getType() == MessageSubType::explorer) {
			if (it == this->echoBuffer.end()) {
				Echos echo;
				echo.EchoID = message->getNumber();
				echo.FirstNeighborID = message->getSourceID();
				echo.counter = 1; // eine Nachricht von einem Nachbar schon erhalten
				this->echoBuffer.insert(EchoEntry(echo.EchoID, echo));

				nodeImpl->process(message);

				Message* explorerMsg = message->prototype();
				explorerMsg->setSourceID(nodeInfo.NodeID);
				sendToDestinations(explorerMsg, message->getSourceID());
				//cout << nodeInfo.NodeID << " - Send Explorer!" << endl;
			} else {
				it->second.counter++;

				//if (it->second.FirstNeighborID != message.getSourceID()) {
					// FEHLER?! Zweimal Explorer vom selben Knoten mit selber ECHO-ID (=MessageNumber)
				//}
			}
		} else if (message->getType() == MessageSubType::echo) {
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
				Message* echoMsg = message->prototype();
				if (echoMsg->getType() != MessageSubType::echo) {
					echoMsg->setType(MessageSubType::echo);
				}

				echoMsg->setSourceID(nodeInfo.NodeID);
				sendTo(echoMsg, this->neighbors.at(it->second.FirstNeighborID));
			}
			if (dynamic_cast<ControlMessage*>(message) != NULL) {
				if (message->getContent() == constants::SHUTDOWN_ECHO) {
					isRunning = !isRunning;
				} else if (helper::utilities::isNumber(message->getContent())) {
					// Terminierungszeit erhalten
					this->vectorTime->setTermininationTime(stoi(message->getContent()));
				}
			}
		}
	} catch (std::exception& e) {
		helper::utilities::writeLog(__FUNCTION__, e);
		throw e;
	}
}

bool NodeCore::sendToDestinationsImpl(Message* const message, const NodeMap& destinations) {
	bool successfully = false;
	for (NodeMap::const_iterator it = destinations.begin(); it != destinations.end(); ++it) {
		successfully = sendTo(message, it->second);
	}
	return successfully;
}

bool NodeCore::sendToDestinations(Message* const message, const int& excludedNodeID) {
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
	Message* explorerMsg = new ApplicationMessage(MessageSubType::explorer, echo.EchoID, nodeInfo.NodeID, content);
	return this->sendToDestinationsImpl(explorerMsg, neighbors);
}

bool NodeCore::sendTo(Message* const message, const int& nodeID) {
	NodeInfo destination;
	this->transceiver->resolve(nodeID, destination);
	return this->sendTo(message, destination);
}

bool NodeCore::sendTo(Message* const message, const NodeInfo& destination) {
	if (dynamic_cast<ApplicationMessage*>(message) != NULL) {
		this->vectorTime->increase();
		if (this->vectorTime->isTerminated()) {
			// sanft abschalten
			isRunning = !isRunning;
		}
	}

	message->setDestinationId(destination.NodeID);
	message->setSourceID(nodeInfo.NodeID);
	message->setVectorTimes(this->vectorTime->getTimeMap());



//	if (message.getType() != MessageType::log)
//		log->push_back(new string(to_string(vectorTime->getLocalTime()) + " Send to "  + to_string(destination.NodeID) + ": " + msg.toString()));

	return transceiver->sendTo(destination, MessageFactory::convertToString(message));
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

	ControlMessage* logMsg = new ControlMessage(MessageSubType::log, 0, nodeInfo.NodeID, string(p.CStr()));
	sendToListener(logMsg);
	doc.Clear();
}

bool NodeCore::sendToListener(Message* const message) {
	message->setVectorTimes(this->vectorTime->getTimeMap());
	return sendTo(message, listenerNodeInfo);
}

bool NodeCore::sendStatusToListener(const string& status) {
	Message* statusMsg = new ControlMessage(MessageSubType::normal, 0, nodeInfo.NodeID, status);
	return sendToListener(statusMsg);
}


} /* namespace node */

