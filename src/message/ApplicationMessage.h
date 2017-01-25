/*
 * ApplicationMessage.h
 *
 *  Created on: 25.01.2017
 *      Author: tobias
 */

#ifndef MESSAGE_APPLICATIONMESSAGE_H_
#define MESSAGE_APPLICATIONMESSAGE_H_

#include <string>

#include "Message.h"

namespace message {

class ApplicationMessage: public Message {
public:
	ApplicationMessage(const MessageSubType& type, const string& content);
	ApplicationMessage(const MessageSubType& type, int number, const string& content);
	ApplicationMessage(const MessageSubType& type, int number, int sourceID, const string& content);
	ApplicationMessage(const ApplicationMessage& message);
	virtual ~ApplicationMessage();

	virtual Message* prototype();

	virtual string toString() const;
};

} /* namespace message */

#endif /* MESSAGE_APPLICATIONMESSAGE_H_ */
