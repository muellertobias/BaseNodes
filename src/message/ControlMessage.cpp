/*
 * ControlMessage.cpp
 *
 *  Created on: 25.01.2017
 *      Author: tobias
 */

#include "ControlMessage.h"

#include <sstream>

namespace message {

ControlMessage::ControlMessage(const MessageSubType& type,
		const string& content) : Message(type, content) {
}

ControlMessage::ControlMessage(const MessageSubType& type, int number,
		const string& content) : Message(type, number, content) {
}

ControlMessage::ControlMessage(const MessageSubType& type, int number,
		int sourceID, const string& content) : Message(type, number, sourceID, content) {
}

ControlMessage::ControlMessage(const ControlMessage& message)
	: ControlMessage(message.type, message.number, message.sourceID, message.content) {
	//this->vectorTime = message.vectorTime; //TODO
	this->destinationID = message.destinationID;
}

ControlMessage::~ControlMessage() {
	// TODO Auto-generated destructor stub
}

Message* ControlMessage::prototype() {
	Message* prototype = new ControlMessage(*this);
	return prototype;
}

string ControlMessage::toString() const {
	stringstream strStream;
	strStream << "ControlMessage - " << Message::toString();
	return strStream.str();
}

} /* namespace message */
