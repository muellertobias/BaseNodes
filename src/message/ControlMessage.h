/*
 * ControlMessage.h
 *
 *  Created on: 25.01.2017
 *      Author: tobias
 */

#ifndef MESSAGE_CONTROLMESSAGE_H_
#define MESSAGE_CONTROLMESSAGE_H_

#include <string>

#include "Message.h"

namespace message {

class ControlMessage: public Message {
public:
	ControlMessage(const MessageSubType& type, const string& content);
	ControlMessage(const MessageSubType& type, int number, const string& content);
	ControlMessage(const MessageSubType& type, int number, int sourceID, const string& content);
	ControlMessage(const ControlMessage& message);
	virtual ~ControlMessage();

	virtual Message* prototype();

	virtual string toString() const;
};

} /* namespace message */

#endif /* MESSAGE_CONTROLMESSAGE_H_ */
