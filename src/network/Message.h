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

	string toString() const {
		string str;
		if (this->type == MessageType::application) {
			str.append("Application ");
		} else if (this->type == MessageType::control) {
			str.append("Control ");
		} else {
			str.append("Undefined ");
		}
		str.append("from ");
		str.append(to_string(this->sourceID));
		str.append(" with number ");
		str.append(to_string(this->number));
		str.append(": ");
		str.append(this->content);
		return str;

	}
private:
	MessageType type;
	int number;
	int sourceID;
	string content;
};


} /* namespace network */

#endif /* NETWORK_MESSAGE_H_ */
