/*
 * NodeListener.cpp
 *
 *  Created on: 11.01.2017
 *      Author: tobias
 */

#include "NodeListener.h"

#include <unistd.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <map>

#include "../helper/Constants.h"

namespace helper {
namespace listener {

NodeListener::NodeListener(TransceiverBase* transceiver) {
	this->transceiver = transceiver;
	this->isRunning = true;
}

NodeListener::~NodeListener() {
	delete this->transceiver;
}

void NodeListener::loop() {

	map<int,string> election;

	while (isRunning) {
		try {
			cout << "Listen..." << endl;
			Message* message = transceiver->receive();
			if (dynamic_cast<ControlMessage*>(message) != NULL) {
				handle((ControlMessage*)message);
			}
			if (message->getContent().find('|') != string::npos && message->getContent().size() <= 7) {
				vector<string> results = split(message->getContent(), "|");
				int left = stoi(results.at(0));
				int right = stoi(results.at(1));
				string choice;
				if (left > right) {
					choice = "L";
				} else if (left < right) {
					choice = "R";
				} else {
					choice = "/";
				}

				map<int,string>::iterator it = election.find(message->getSourceID());
				if (it != election.end()) {
					it->second = choice;
				} else {
					election.insert(pair<int,string>(message->getSourceID(), choice));
				}
			} else {
				print(message);
			}
		} catch (std::exception& ex) {
			cerr << ex.what() << endl;
		}

	}

	std::stringstream log;
	for (map<int,string>::iterator it = election.begin(); it != election.end(); ++it) {
		log << it->first << "->" << it->second << endl;
	}


	std::stringstream logfilename;
	logfilename << "election_" << getpid() << ".txt";
	std::ofstream logFile;
	logFile.open(logfilename.str(), std::ios::app);
	logFile << log.str();
	logFile.close();
}

void NodeListener::print(Message* const message) {
	std::stringstream log;
	log << message->getSourceID() << " - " << message->getContent() << endl;
	cout << log.str();

	std::stringstream logfilename;
	logfilename << "listener_" << getpid() << ".txt";
	std::ofstream logFile;
	logFile.open(logfilename.str(), std::ios::app);
	logFile << log.str();
	logFile.close();
}

void NodeListener::handle(ControlMessage* message) {
	if (message->getContent() == constants::SHUTDOWN) {
		this->isRunning = false;
	}
}

} /* namespace listener */
} /* namespace helper */
