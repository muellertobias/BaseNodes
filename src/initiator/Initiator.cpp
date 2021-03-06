/*
 * Initiator.cpp
 *
 *  Created on: 11.11.2016
 *      Author: tobias
 */

#include "Initiator.h"

#include "../helper/utilities/utilities.h"

Initiator::Initiator(Settings* settings)
	: nodes(settings->getNeighbors()), transceiver(settings->getTransceiver()) {
}

void Initiator::loop() {
	showDetails();
	bool isRunning = true;
	do {
		try {
		cout << "> ";
		string input = readInput();
		if (input == "exit") {
			isRunning = false;
		} else if (input == "help") {
			showDetails();
		} else if (input == "start") {
			tell();
		} else if (input == "halt") {
			halt();
		} else if (input == "snap") {
			snapshoot();
		} else if (input == "echo") {
			sendEchoShutdown();
		} else if (input == "time") {
			sendInit();
		} else if (input == "reset") {
			sendReset();
		}
		} catch (const int& nodeID) {
			cout << nodeID << " nicht gefunden!" << endl;
		} catch (const std::exception& ex) {
			cout << "Falsche Eingabe!" << endl;
		} catch (const string& ex) {
			cout << ex << " ist keine Zahl!" << endl;
		}
	} while (isRunning);
}

void Initiator::showDetails() {
	cout << "Befehle:\nhelp - Zeige Hilfe an\n" <<
			"exit - Beenden\n" <<
			"start - Löse Initialnachricht an beide Kandidaten aus\n" <<
			"snap - Erzeuge Schnappschuss\n" <<
			"halt - Halte einen/alle Knoten an\n" <<
			"echo - Hält alle Knoten an mittels ECHO\n" <<
			"time - Setzt Terminierungszeit\n" <<
			"reset - Setzt ausgewählten Knoten zurück\n" << endl;
	cout << "Mögliche Adressen:" << endl;

	for (NodeMap::const_iterator it = nodes.begin(); it != nodes.end(); ++it) {
		cout << "\t" << it->first << endl;
	}
}

string Initiator::readInput() const {
	string input;
	getline(cin, input);
	return input;
}

NodeInfo Initiator::readNodeID() {
	int nodeID;
	try {
		cout << "Node > ";
		nodeID = stoi(readInput());
		NodeInfo nodeInfo = nodes.at(nodeID);
		cout << nodeInfo.NodeID << " gefunden!" << endl;
		return nodeInfo;
	} catch (const out_of_range& ex) {
		cout << nodeID << " nicht gefunden!" << endl;
		throw nodeID;
	}
	return NodeInfo();
}

void Initiator::tell() {
	cout << "Geben Sie die Kandidaten nacheinander ein!" << endl;
	NodeInfo nodeInfo1 = readNodeID();
	NodeInfo nodeInfo2 = readNodeID();

	string content = "START";
	Message* message = new ControlMessage(MessageSubType::normal, 0, content);
	sendTo(message, nodeInfo1);
	sendTo(message, nodeInfo2);
}

void Initiator::sendEchoShutdown() {
	NodeInfo nodeInfo = readNodeID();

	Message* message = new ControlMessage(MessageSubType::normal, 0, constants::SHUTDOWN_ECHO);

	sendTo(message, nodeInfo);
}

void Initiator::halt() {
	NodeInfo nodeInfo = readNodeID();

	int sourceID = nodeInfo.NodeID;
	cout << "Alle Nodes anhalten? (y/n)> ";
	string shutdownCommand;
	if (readInput() == "y") {
		shutdownCommand = constants::SHUTDOWN_ALL;
	} else {
		shutdownCommand = constants::SHUTDOWN;
	}
	Message* message = new ControlMessage(MessageSubType::normal, sourceID, shutdownCommand);
	sendTo(message, nodeInfo);
}

void Initiator::snapshoot() {
	const NodeInfo& nodeInfo = readNodeID();
	Message* message = new ControlMessage(MessageSubType::normal, constants::SNAPSHOT);
	sendTo(message, nodeInfo);
}

void Initiator::sendInit() {
	NodeInfo nodeInfo = readNodeID();

	cout << "Terminationzeit? > ";
	string terminationTime = readInput();
	if (!helper::utilities::isNumber(terminationTime)) {
		throw terminationTime;
	}

	Message* message = new ControlMessage(MessageSubType::parametrize, terminationTime);
	sendTo(message, nodeInfo);
}

void Initiator::sendReset() {
	cout << "Nicht implementiert" << endl;
}

void Initiator::sendTo(Message* const message, const NodeInfo& nodeInfo) {
	if (this->transceiver->sendTo(nodeInfo, MessageFactory::convertToString(message))) {
		cout << "Erfolgreich!" << endl;
	} else {
		cout << "Fehlgeschlagen!" << endl;
	}
}
