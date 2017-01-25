/*
 * Message.h
 *
 *  Created on: 01.11.2016
 *      Author: tobias
 */

#ifndef MESSAGE_MESSAGE_H_
#define MESSAGE_MESSAGE_H_

#include <string>

#include "../helper/time/VectorTime.h"

namespace message {

using namespace std;
using namespace helper::time;

enum MessageSubType {
	undefined,
	normal,
	log,
	explorer,
	echo
};

class Message {
public:
	Message(const MessageSubType& type, const string& content);
	Message(const MessageSubType& type, int number, const string& content);
	Message(const MessageSubType& type, int number, int sourceID, const string& content);

	virtual ~Message();

	virtual Message* prototype() = 0;

	const MessageSubType& getType() const {
		return type;
	}

	void setType(MessageSubType type) {
		this->type = type;
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

	virtual string toString() const;

	bool setVectorTime(const int& nodeID, const int& time);

	const VectorTimeMap& getVectorTimes() const {
		return vectorTime.getTimeMap();
	}

	void setVectorTimes(const VectorTimeMap& vectorTimes) {
		this->vectorTime.merge(vectorTimes);
	}

	int getDestinationId() const {
		return destinationID;
	}

	void setDestinationId(int destinationId) {
		destinationID = destinationId;
	}

protected:
	MessageSubType type;
	int number;
	int sourceID;
	int destinationID;
	string content;
	VectorTime vectorTime;
};


} /* namespace network */

#endif /* MESSAGE_MESSAGE_H_ */
