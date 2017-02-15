/*
 * MutualFileAccessNodeCoreImpl.cpp
 *
 *  Created on: 14.02.2017
 *      Author: tobias
 */

#include "MutualFileAccessNodeCoreImpl.h"

#include <fstream>
#include <iostream>
#include <map>

#include "../../../message/ApplicationMessage.h"
#include "../../../message/ControlMessage.h"
#include "../../../message/Message.h"
#include "../../NodeCore.h"


namespace core {
namespace implementation {
namespace mutual {

MutualFileAccessNodeCoreImpl::MutualFileAccessNodeCoreImpl(int highestNodeID) {
	queue = new RequestQueue(highestNodeID);
	terminiationCounter = 0;
	this->highestNodeID = highestNodeID;
}

MutualFileAccessNodeCoreImpl::~MutualFileAccessNodeCoreImpl() {
	delete queue;
}

void MutualFileAccessNodeCoreImpl::process(Message* const message) {
	if (dynamic_cast<ControlMessage*>(message) != NULL && message->getContent() == "shutdown") {
		int destination = -1;
		ControlMessage* haltMessage = new ControlMessage(MessageSubType::normal, "shutdown");
		if (core->getNodeInfo().NodeID % 2 == 0) {
			destination = core->getNodeInfo().NodeID - 1;
		} else {
			destination = core->getNodeInfo().NodeID + 1;
		}
		if (destination <= highestNodeID)
			sendTo(haltMessage, destination);
		return;
	}
	if (message->getSourceID() == 0) {
		ApplicationMessage* requestMessage = new ApplicationMessage(MessageSubType::normal, "REQUEST");
		sendToAll(requestMessage, 0);
		return;
	}

	if (message->getSourceID() != core->getNodeInfo().NodeID) {
		if (dynamic_cast<ApplicationMessage*>(message) != NULL) {
			if (message->getContent() == "REQUEST") {
				Request* request = new Request();
				request->NodeID = message->getSourceID();
				request->Timestamp = message->getVectorTimes().at(message->getSourceID());
				queue->push(request);
				ApplicationMessage* ackMessage = new ApplicationMessage(MessageSubType::normal, "ACK");
				sendTo(ackMessage, message->getSourceID());
			} else if (message->getContent() == "ACK") {

			} else if (message->getContent() == "RELEASE") {
				queue->remove(message->getSourceID());
			}
			if (queue->topIsFrom(core->getNodeInfo().NodeID) && queue->containsRequestFromAll()) {
				// Critical Section!
				criticalSection();
				//...
				ApplicationMessage* releaseMessage = new ApplicationMessage(MessageSubType::normal, "RELEASE");
				sendToAll(releaseMessage, 0);
				queue->remove(core->getNodeInfo().NodeID);
			}
		}
		ApplicationMessage* requestMessage = new ApplicationMessage(MessageSubType::normal, "REQUEST");
		sendToAll(requestMessage, 0);
	} else {
		Request* request = new Request();
		request->NodeID = message->getSourceID();
		request->Timestamp = message->getVectorTimes().at(message->getSourceID());
		queue->push(request);
	}

}

void MutualFileAccessNodeCoreImpl::getState(string& state) {
}

void MutualFileAccessNodeCoreImpl::criticalSection() {
	fstream file;
	file.open("X.txt", ios::in | ios::out);

	string firstLine;
	getline(file, firstLine);
	int value = stoi(firstLine);

	if (value == 0) {

	}
	//terminiationCounter++;
	if (terminiationCounter == 3) {
		int destination = -1;
		ControlMessage* haltMessage = new ControlMessage(MessageSubType::normal, "shutdown");
		if (core->getNodeInfo().NodeID % 2 == 0) {
			destination = core->getNodeInfo().NodeID - 1;
		} else {
			destination = core->getNodeInfo().NodeID + 1;
		}
		if (destination <= highestNodeID)
			sendTo(haltMessage, destination);

	} else {
		if (core->getNodeInfo().NodeID % 2 == 0) {
			value--;
			if (value < 0)
				value = 0;
		} else {
			value++;
		}
		cout << core->getNodeInfo().NodeID << " - CS! - " << value << endl;
		file.seekp(0);
		file.fill('0');
		file.width(9);
		file << value;
		file.seekg(0, ios::end);
		file << "\n" << core->getNodeInfo().NodeID;
	}

	file.close();
}

} /* namespace mutual */
} /* namespace implementation */
} /* namespace core */
