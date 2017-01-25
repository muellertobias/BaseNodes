/*
 * ApplicationMessage.cpp
 *
 *  Created on: 25.01.2017
 *      Author: tobias
 */

#include "ApplicationMessage.h"

#include <sstream>

namespace message {

ApplicationMessage::ApplicationMessage(const MessageSubType& type,
		const string& content) : Message(type, content) {
}

ApplicationMessage::ApplicationMessage(const MessageSubType& type, int number,
		const string& content) : Message(type, number, content) {
}

ApplicationMessage::ApplicationMessage(const MessageSubType& type, int number,
		int sourceID, const string& content) : Message(type, number, sourceID, content) {
}


ApplicationMessage::ApplicationMessage(const ApplicationMessage& message)
	: ApplicationMessage(message.type, message.number, message.sourceID, message.content) {
	//this->vectorTime = message.vectorTime;
	this->destinationID = message.destinationID;
}

ApplicationMessage::~ApplicationMessage() {
	// TODO Auto-generated destructor stub
}

Message* ApplicationMessage::prototype() {
	Message* prototype = new ApplicationMessage(*this);
	return prototype;
}

string ApplicationMessage::toString() const {
	stringstream strStream;
	strStream << "ApplicationMessage - " << Message::toString();
	return strStream.str();
}

} /* namespace message */


