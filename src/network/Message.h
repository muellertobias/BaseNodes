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
#include "VectorTime.h"


using namespace std;


namespace network {

using namespace helper::time;

enum MessageType {
	undefined,
	control,
	application,
	log
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

	bool setVectorTime(const int& nodeID, const int& time);

	const VectorTimeMap& getVectorTimes() const {
		return vectorTime.getTimeMap();
	}

	void setVectorTimes(const VectorTimeMap& vectorTimes) {
		this->vectorTime.merge(vectorTimes);
	}

private:

	MessageType type;
	int number;
	int sourceID;
	//int destinationID;
	string content;
	VectorTime vectorTime;
};


} /* namespace network */

#endif /* NETWORK_MESSAGE_H_ */
