/*
 * Initiator.cpp
 *
 *  Created on: 11.11.2016
 *      Author: tobias
 */

#include "Initiator.h"
#include <iostream>
#include "helper/randomizer/Random.h"
#include "network/Message.h"

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
		}
	} while (isRunning);
}

void Initiator::showDetails() {
	cout << "Befehle:\nhelp - Zeige Hilfe an\nexit - Beenden\ntell - Löse Initialnachricht aus\nhalt - Halte einen/alle Knoten an" << endl;
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
	int nodeID = stoi(readInput());

	try {
		NodeInfo nodeInfo = nodes.at(nodeID);
		cout << nodeInfo.NodeID << " founded!" << endl;

		cout << "Nachrichtentext > ";
		string content = "Init|" + readInput();

		if (sendTo(content, nodeInfo, 0)) {
			cout << "Successful!" << endl;
		} else {
			cout << "Failed!" << endl;
		}

	} catch (const out_of_range& ex) {
		cout << nodeID << " not founded!" << endl;
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

			if (sendTo("Shutdown", nodeInfo, sourceID)) {
				cout << "Successful!" << endl;
			} else {
				cout << "Failed!" << endl;
			}
	} catch (const out_of_range& ex) {
		cout << nodeID << " not founded!" << endl;
	}
}

bool Initiator::sendTo(const string& content, const NodeInfo& nodeInfo, int sourceID) {
	int number = helper::randomizer::random(0, 9999);

	Message message(MessageType::control, number, sourceID, content);
	return this->core.sendTo(message, nodeInfo);
}
