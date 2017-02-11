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
	while (isRunning) {
		cout << "Listen..." << endl;
		Message* message = transceiver->receive();
		handle((ControlMessage*)message);
		print(message);
	}
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
