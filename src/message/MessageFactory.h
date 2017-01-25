/*
 * MessageFactory.h
 *
 *  Created on: 25.01.2017
 *      Author: tobias
 */

#ifndef MESSAGE_MESSAGEFACTORY_H_
#define MESSAGE_MESSAGEFACTORY_H_

#include <string>
#include "../helper/utilities/tinyxml2.h"
#include "Message.h"

namespace message {

using namespace std;
using namespace tinyxml2;


class MessageFactory {
public:
	virtual ~MessageFactory() {}

	static Message* create(const string& xmlRawData);
	static string convertToString(Message* const message);

private:
	MessageFactory() {}
	MessageFactory(const MessageFactory&) {}

	static Message* read(const string& xmlRawData);
	static string write(Message* const message);
	static tinyxml2::XMLElement* writeXMLElement(Message* const message, XMLDocument& doc);
};

} /* namespace message */

#endif /* MESSAGE_MESSAGEFACTORY_H_ */
