/*
 * Message.h
 *
 *  Created on: 01.11.2016
 *      Author: tobias
 */

#ifndef NETWORK_MESSAGE_H_
#define NETWORK_MESSAGE_H_

#include <iostream>
#include <string>

#include "Serializable.h"
#include "../helper/utilities/tinyxml2.h"


using namespace std;

namespace network {

enum MessageType {
	undefined,
	control,
	application
};

class Message : public Serializable {
public:
	Message(const string& str);
	Message(const MessageType& type, int number, int sourceID, const string& content);
	virtual ~Message();

	const MessageType& getType() const {
		return type;
	}

	int getNumber() const {
		return number;
	}

	int getSourceID() const {
		return sourceID;
	}

	void setSourceID(int sourceID) {
		this->sourceID = sourceID;
	}

	const string& getContent() const {
		return content;
	}

	virtual bool read(const string& str);
	virtual string write() const;
	virtual tinyxml2::XMLElement* writeXMLElement(tinyxml2::XMLDocument& doc) const;

	string toString() const;

private:
	MessageType type;
	int number;
	int sourceID;
	string content;
};


} /* namespace network */

#endif /* NETWORK_MESSAGE_H_ */
