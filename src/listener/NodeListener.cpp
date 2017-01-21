/*
 * NodeListener.cpp
 *
 *  Created on: 11.01.2017
 *      Author: tobias
 */

#include "NodeListener.h"

#include "../helper/utilities/tinyxml2.h"

namespace helper {
namespace listener {

using namespace tinyxml2;

NodeListener::NodeListener(TransceiverBase* transceiver) {
	this->transceiver = transceiver;
}

NodeListener::~NodeListener() {
	// TODO Auto-generated destructor stub
}

void NodeListener::loop() {
	while (true) {
		cout << "Listen..." << endl;
		Message message = receive();
		print(message);
	}
}

Message NodeListener::receive() const {
	string buffer = transceiver->receive();
	return Message(buffer);
}

void NodeListener::print(const Message& message) {
	if (message.getType() == MessageType::log) {
		string content = message.getContent();
		// XML
		XMLDocument doc;
		doc.Parse(content.c_str(), content.length());

		XMLElement* root = doc.FirstChild()->ToElement();

		int length = 0;
		root->QueryAttribute("length", &length);

		for (XMLElement* element = root->FirstChildElement("logEntry"); element != NULL; element = element->NextSiblingElement("logEntry")) {
			cout << element->GetText() << endl;
		}
	} else if (message.getType() == MessageType::application){
		cout << message.getContent() << endl;
	}
}

} /* namespace listener */
} /* namespace helper */
