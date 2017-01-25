/*
 * Message.cpp
 *
 *  Created on: 01.11.2016
 *      Author: tobias
 */

#include "Message.h"

#include <sstream>

#include "../helper/randomizer/Random.h"

namespace message {

Message::Message(const MessageSubType& type, const string& content)
	: Message(type, helper::randomizer::random(0, 9999), 0, content) {
}

Message::Message(const MessageSubType& type, int number, const string& content)
	: Message(type, number, 0, content) {
}

Message::Message(const MessageSubType& type, int number ,int sourceID, const string& content)
	: type(type), number(number), sourceID(sourceID), destinationID(-1), content(content) {
}

Message::~Message() {
}

string Message::toString() const {
	stringstream strStream;

	if (this->type == MessageSubType::normal) {
		strStream << "Normal -";
	} else if (this->type == MessageSubType::explorer) {
		strStream << "Explorer -";
	} else if (this->type == MessageSubType::echo) {
			strStream << "Echo -";
	} else if (this->type == MessageSubType::log) {
		strStream << "Logging -";
	} else {
		strStream << "Undefined -";
	}
	strStream << " Number=" << this->number << " SourceID=" << this->sourceID << " DestinationID=" << this->destinationID;
	strStream << " Content={" << this->content << "} VectorTimes={";

	for (VectorTimeMap::const_iterator it = vectorTime.getTimeMap().begin(); it != vectorTime.getTimeMap().end(); it++) {
		strStream << "[" << (*it).first << "," << (*it).second << "]";
	}
	strStream <<  "}";

	return strStream.str();
	}

bool Message::setVectorTime(const int& nodeID, const int& time) {
	vectorTime.setTime(nodeID, time);

	return true;

}

} /* namespace message */
