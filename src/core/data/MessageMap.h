/*
 * MessageMap.h
 *
 *  Created on: 09.11.2016
 *      Author: tobias
 */

#ifndef CORE_DATA_MESSAGEMAP_H_
#define CORE_DATA_MESSAGEMAP_H_

#include <map>
#include "../../message/Message.h"
#include <string>

using namespace network;

typedef int MessageSource;
typedef int MessageNumber;
typedef std::pair<MessageNumber, Message> MessagePair; // Number of Message and Message itself
typedef std::map<MessagePair::first_type, MessagePair::second_type> MessageMap;

#endif /* CORE_DATA_MESSAGEMAP_H_ */
