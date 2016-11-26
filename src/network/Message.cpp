/*
 * Message.cpp
 *
 *  Created on: 01.11.2016
 *      Author: tobias
 */

#include "Message.h"
#include "../helper/string/trim.h"

namespace network {

Message::Message(const string& str) {
	if (!read(str)) {
		cerr << "Invalid argument!" << endl;
		//throw new invalid_argument("str"); // Austauschen gegen eigene Exception beim decoden
	}
}

Message::Message(const MessageType& type, int number ,int sourceID, const string& content)
	: type(type), number(number), sourceID(sourceID),  content(content) {
}

Message::~Message() {
}

bool Message::read(const string& str) {
	vector<string> splittedString = split(str, DELIMITER);

	if (splittedString.size() == 0) {
		return false;
	}

	if (splittedString[0] != "Message") {
		return false;
	}

	try {
		this->type = static_cast<MessageType>(stoi(splittedString[1]));
		this->number = stoi(splittedString[2]);
		this->sourceID = stoi(splittedString[3]);
		this->content = splittedString[4];
	} catch (const invalid_argument& ex) {
		return false;
	}

	return true;
}

string Message::write() const {
	string returnString;
	returnString.append("Message");
	returnString.append(DELIMITER);
	returnString.append(to_string(type));
	returnString.append(DELIMITER);
	returnString.append(to_string(number));
	returnString.append(DELIMITER);
	returnString.append(to_string(sourceID));
	returnString.append(DELIMITER);
	returnString.append(content);

	return returnString;
}

} /* namespace network */
