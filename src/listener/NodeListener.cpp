/*
 * NodeListener.cpp
 *
 *  Created on: 11.01.2017
 *      Author: tobias
 */

#include "NodeListener.h"

#include <iostream>
#include <string>

#include "../helper/Constants.h"
#include "../helper/utilities/tinyxml2.h"
#include "../message/MessageFactory.h"

namespace helper {
namespace listener {

using namespace tinyxml2;

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
		Message* message = receive();
		handle((ControlMessage*)message);
		print(message);
	}
}

Message* NodeListener::receive() const {
	string buffer = transceiver->receive();
	return MessageFactory::create(buffer);
}

void NodeListener::print(Message* const message) {
	if (message->getType() == MessageSubType::log) {
		string content = message->getContent();
		// XML
		XMLDocument doc;
		doc.Parse(content.c_str(), content.length());

		XMLElement* root = doc.FirstChild()->ToElement();

		int length = 0;
		root->QueryAttribute("length", &length);

		for (XMLElement* element = root->FirstChildElement("logEntry"); element != NULL; element = element->NextSiblingElement("logEntry")) {
			cout << element->GetText() << endl;
		}
	} else {
		cout << message->getContent() << endl;
	}
}

void NodeListener::handle(ControlMessage* message) {
	if (message->getContent() == constants::SHUTDOWN) {
		this->isRunning = !isRunning;
	}
}

} /* namespace listener */
} /* namespace helper */
