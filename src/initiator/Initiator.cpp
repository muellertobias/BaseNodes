/*
 * Initiator.cpp
 *
 *  Created on: 11.11.2016
 *      Author: tobias
 */

#include "Initiator.h"

#include <iostream>
#include <map>

#include "../helper/Constants.h"
#include "../helper/randomizer/Random.h"

Initiator::Initiator(const NodeCore& core)
: core(core), nodes(core.getNeighbors()) {
}

void Initiator::loop() {
	showDetails();
	bool isRunning = true;
	do {
		cout << "> ";
		string input = readInput();
		if (input == "exit") {
			isRunning = false;
		} else if (input == "help") {
			showDetails();
		} else if (input == "tell") {
			tell();
		} else if (input == "halt") {
			halt();
		} else if (input == "snap") {
			snapshoot();
		}
	} while (isRunning);
}

void Initiator::showDetails() {
	cout << "Befehle:\nhelp - Zeige Hilfe an\n" <<
			"exit - Beenden\n" <<
			"tell - Löse Initialnachricht aus\n" <<
			"snap - Erzeuge Schnappschuss\n" <<
			"halt - Halte einen/alle Knoten an" << endl;
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

void Initiator::tell() {
	cout << "Node > ";
	int nodeID = 0;

	try {
		nodeID = stoi(readInput());

		NodeInfo nodeInfo = nodes.at(nodeID);
		cout << nodeInfo.NodeID << " gefunden!" << endl;

		cout << "Nachrichtentext > ";
		string content = readInput();

		int number = helper::randomizer::random(0, 9999);
		Message message(MessageType::control, number, 0, content);

		if (sendTo(message, nodeInfo)) {
			cout << "Erfolgreich!" << endl;
		} else {
			cout << "Fehlgeschlagen!" << endl;
		}

	} catch (const out_of_range& ex) {
		cout << nodeID << " nicht gefunden!" << endl;
	} catch (const std::exception& ex) {
		cout << "Falsche Eingabe!" << endl;
	}
}

void Initiator::halt() {
	cout << "Inititiator Node > ";
	int nodeID = stoi(readInput());

	try {
		NodeInfo nodeInfo = nodes.at(nodeID);
		cout << nodeInfo.NodeID << " gefunden!" << endl;

		int sourceID = nodeID;
		cout << "Alle Nodes anhalten? (y/n)> ";
		if (readInput() == "y") {
			sourceID = -1;
		}

		if (sendTo(constants::ShutdownMessage, nodeInfo, sourceID)) {
			cout << "Erfolgreich!" << endl;
		} else {
			cout << "Fehlgeschlagen!" << endl;
		}
	} catch (const out_of_range& ex) {
		cout << nodeID << " nicht gefunden!" << endl;
	}
}

void Initiator::snapshoot() {
	cout << "Node > ";
	int nodeID = stoi(readInput());
	try {
		NodeInfo nodeInfo = nodes.at(nodeID);
		cout << nodeInfo.NodeID << " gefunden!" << endl;

		if (sendTo("Snapshot", nodeInfo, 0)) {
			cout << "Erfolgreich!" << endl;
		} else {
			cout << "Fehlgeschlagen!" << endl;
		}
	} catch (const out_of_range& ex) {
		cout << nodeID << " nicht gefunden!" << endl;
	}

}

bool Initiator::sendTo(const string& content, const NodeInfo& nodeInfo, int sourceID) {
	int number = helper::randomizer::random(0, 9999);

	Message message(MessageType::control, number, sourceID, content);
	return this->core.sendTo(message, nodeInfo);
}

bool Initiator::sendTo(const Message& message, const NodeInfo& nodeInfo) {
	return this->core.sendTo(message, nodeInfo);
}
