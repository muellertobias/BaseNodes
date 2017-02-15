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
#include <climits>

#include "../helper/Constants.h"
#include "../helper/exception/NetworkException.h"
#include "../helper/exception/ShuttingDownException.h"
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

	auto sendEchoFunction = [this](const string& content) -> bool { return this->sendEcho(content); };
	this->nodeImpl->setSendEcho(sendEchoFunction);

	auto sendToFunction = [this](Message* msg, const int& nodeID) -> bool { return this->sendTo(msg, nodeID); };
	this->nodeImpl->setSendTo(sendToFunction);

	auto sendToAllFunction = [this](Message* const msg, const int& excludedID) -> bool { return this->sendToDestinations(msg, excludedID); };
	this->nodeImpl->setSendToDestinations(sendToAllFunction);

	// Vectortime
	vectorTime = new VectorTime(this->nodeInfo.NodeID);

	// Control
	isShuttingDown = false;

	// Snapshot
	pastImpl = NULL;
	isMarked = false;
}

NodeCore::~NodeCore() {
	delete transceiver;
	delete nodeImpl;
	if (pastImpl != NULL) {
		delete pastImpl;
	}
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

				delete message;
			} catch (const helper::exception::ShuttingDownException& ex) {
				//helper::utilities::writeLog(__FUNCTION__, "Ignore Application Message");
			} catch (const helper::exception::NetworkException& ex) {
				helper::utilities::writeLog(__FUNCTION__, ex);
			} catch (std::exception& e) {
				//throw e;
			}
		}
	} catch (std::exception& e) {
		helper::utilities::writeLog(__FUNCTION__, e);
	}
	sendStatusToListener("stopped");
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
	string buffer;
	nodeImpl->getState(buffer);
	strStream << "Impl-State: " << buffer << endl;
	sendStatusToListener(strStream.str());
}

Message* NodeCore::receive() {
	try {
		Message* msg = transceiver->receive();
		if (dynamic_cast<ApplicationMessage*>(msg) != NULL) {
			if (isShuttingDown) {
				throw helper::exception::ShuttingDownException();
			}
			//this->vectorTime->merge(msg->getVectorTimes());
			//this->vectorTime->setTime(this->nodeInfo.NodeID, this->vectorTime->getMaximum());
			this->vectorTime->increase();
			if (this->vectorTime->isTerminated() && !isMarked) {
				createTemporaryPastImplementation();
			}
		}

		return msg;
	} catch (helper::exception::ShuttingDownException& e) {
		throw e;
	} catch (std::exception& e) {
		helper::utilities::writeLog(__FUNCTION__, e);
		throw e;
	}
}

void NodeCore::handleControlMessage(ControlMessage* const message) {
	try {
		const string& content = message->getContent();

		if (content == constants::SHUTDOWN) {
			isRunning = false;
		} else if (content == constants::SHUTDOWN_ALL) {
			Message* shutdownMsg = new ControlMessage(MessageSubType::normal, message->getNumber(), nodeInfo.NodeID, constants::SHUTDOWN_ALL);
			this->sendToDestinationsImpl(shutdownMsg, neighbors);
			isRunning = false;
		} else if (content == constants::SHUTDOWN_ECHO) {
			initilizeControlEchoMessage(message->getNumber(), constants::SHUTDOWN_ECHO);
		} else if (content == constants::RESET) {
			// Resette Implementation und Vectorzeit
		} else if (message->getType() == MessageSubType::parametrize){
			if (helper::utilities::isNumber(message->getContent())) {
				initilizeControlEchoMessage(message->getNumber(), message->getContent());
			}
		} else {
			// Initial message
			nodeImpl->process(message);
		}
	} catch (std::exception& e) {
		helper::utilities::writeLog(__FUNCTION__, e);
		throw e;
	}
}

void NodeCore::handleApplicationMessage(ApplicationMessage* const message) {
	try {
		processImplementations(message);
	} catch (std::exception& e) {
		helper::utilities::writeLog(__FUNCTION__, e);
		throw e;
	}
}

void NodeCore::processImplementations(Message* const message) {
	nodeImpl->process(message);
	// Termination Time reached and message is from Past
	if (isMarked && VectorTime::getMaximum(message->getVectorTimes()) <= this->vectorTime->getTermininationTime()) {
		pastImpl->process(message);
		// send SNAPSHOT
		string status;
		//status.append(to_string(nodeInfo.NodeID) + "-");
		pastImpl->getState(status);
		sendStatusToListener(status);
	}
}

void NodeCore::handleEchoMessage(Message* const message) {
	try {
		EchoBuffer::iterator it = this->echoBuffer.find(message->getNumber());

		if (message->getType() == MessageSubType::explorer) {
			if (it == this->echoBuffer.end()) {
				//cout << nodeInfo.NodeID << " - first ECHO " << message->getNumber() << endl;
				Echos echo;
				echo.EchoID = message->getNumber();
				echo.FirstNeighborID = message->getSourceID();
				echo.counter = 1; // eine Nachricht von einem Nachbar schon erhalten
				this->echoBuffer.insert(EchoEntry(echo.EchoID, echo));

				processFirstEchoMessage(message);

				Message* explorerMsg = message->prototype();
				explorerMsg->setSourceID(nodeInfo.NodeID);
				sendToDestinations(explorerMsg, message->getSourceID());
			} else {
				it->second.counter++;
			}
		} else if (message->getType() == MessageSubType::echo) {
			it->second.counter++;
		}

		if (it->second.counter == this->neighbors.size()) {
			if (it->second.FirstNeighborID == nodeInfo.NodeID) {
				//ECHO successful
				processImplementations(message);
			} else {
				Message* echoMsg = message->prototype();
				echoMsg->setType(MessageSubType::echo); // change type from Explorer to Echo

				echoMsg->setSourceID(nodeInfo.NodeID);
				sendTo(echoMsg, this->neighbors.at(it->second.FirstNeighborID));
			}
			if (dynamic_cast<ControlMessage*>(message) != NULL) {
				if (message->getContent() == constants::SHUTDOWN_ECHO) {
					isRunning = false; // Neighbors are shutting down and so i can shutdown...
				} else if (helper::utilities::isNumber(message->getContent())) {
					// (Re-)Set Termination Time
					this->vectorTime->setTermininationTime(stoi(message->getContent()));
					isMarked = false;
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
	if (dynamic_cast<ApplicationMessage*>(message) != NULL) {
		this->vectorTime->increase();
	}
	message->setVectorTimes(this->vectorTime->getTimeMap());

	message->setSourceID(nodeInfo.NodeID);

	if (dynamic_cast<ApplicationMessage*>(message) != NULL) {
		this->nodeImpl->process(message); // to put request in requestqueue
	}

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
	echo.EchoID = randomizer::random(0, INT_MAX);
	echo.FirstNeighborID =  nodeInfo.NodeID;
	echo.counter = 0;
	this->echoBuffer.insert(EchoEntry(echo.EchoID, echo));
	Message* explorerMsg = new ApplicationMessage(MessageSubType::explorer, echo.EchoID, nodeInfo.NodeID, content);
	return this->sendToDestinationsImpl(explorerMsg, neighbors);
}

bool NodeCore::sendTo(Message* const message, const int& nodeID) {
	NodeInfo destination;
	if (dynamic_cast<ApplicationMessage*>(message) != NULL) {
		this->vectorTime->increase();
	}
	message->setVectorTimes(this->vectorTime->getTimeMap());

	this->transceiver->resolve(nodeID, destination);
	return this->sendTo(message, destination);
}

bool NodeCore::sendTo(Message* const message, const NodeInfo& destination) {
	message->setDestinationId(destination.NodeID);

	message->setSourceID(nodeInfo.NodeID);

	return transceiver->sendTo(destination, MessageFactory::convertToString(message));
}

bool NodeCore::sendStatusToListener(const string& status) {
//	Message* statusMsg = new ControlMessage(MessageSubType::normal, 0, nodeInfo.NodeID, status);
//	statusMsg->setVectorTimes(this->vectorTime->getTimeMap());
//	return sendTo(statusMsg, listenerNodeInfo);
	cout << nodeInfo.NodeID << " - " << status << endl;
	return true;
}

void NodeCore::createTemporaryPastImplementation() {
	if (pastImpl != NULL) {
		delete pastImpl;
	}
	pastImpl = nodeImpl->prototype();
	isMarked = true;
	string status;
	pastImpl->getState(status);
	sendStatusToListener(status);
}

void NodeCore::initilizeControlEchoMessage(const int& echoID, const string& content) {
	Echos echo;
	echo.EchoID = echoID;
	echo.FirstNeighborID = nodeInfo.NodeID;
	echo.counter = 0; // Ursprung des ECHOs
	this->echoBuffer.insert(EchoEntry(echo.EchoID, echo));
	Message* explorerMsg = new ControlMessage(MessageSubType::explorer, echoID, nodeInfo.NodeID, content);
	sendToDestinationsImpl(explorerMsg, neighbors);
}


void NodeCore::processFirstEchoMessage(Message* const message) {
	if (dynamic_cast<ControlMessage*>(message) != NULL) {
		if (message->getContent() == constants::SHUTDOWN_ECHO) {
			isShuttingDown = true;
			// Set Dummy-Send-Functions
			auto sendEchoFunction = [](const string&) -> bool { return true; };
			this->nodeImpl->setSendEcho(sendEchoFunction);

			auto sendToFunction = [](Message*, const int&) -> bool { return true; };
			this->nodeImpl->setSendTo(sendToFunction);

			auto sendToAllFunction = [](Message* const, const int&) -> bool { return true; };
			this->nodeImpl->setSendToDestinations(sendToAllFunction);
		}
	}
	processImplementations(message);
}

} /* namespace node */

