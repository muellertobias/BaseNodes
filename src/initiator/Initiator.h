/*
 * Initiator.h
 *
 *  Created on: 11.11.2016
 *      Author: tobias
 */

#ifndef INITIATOR_INITIATOR_H_
#define INITIATOR_INITIATOR_H_

#include <string>

#include "../message/Message.h"
#include "../helper/NodeInfo.h"
#include "../network/TransceiverBase.h"
#include <iostream>
#include <map>
#include <stdexcept>

#include "../helper/Constants.h"
#include "../helper/randomizer/Random.h"
#include "../helper/settings/Settings.h"
#include "../message/ControlMessage.h"
#include "../message/MessageFactory.h"


using namespace std;
using namespace message;
using namespace helper::settings;
using namespace network;


class Initiator {
public:
	Initiator(Settings* settings);
	virtual ~Initiator() { }

	void loop();
private:
	NodeMap nodes;
	TransceiverBase* transceiver;

	void showDetails();
	string readInput() const;
	void tell();
	void halt();
	void echo();
	void snapshoot();
	bool sendTo(const string& content, const NodeInfo& nodeID, int sourceID);
	bool sendTo(Message* const message, const NodeInfo& nodeID);
};


#endif /* INITIATOR_INITIATOR_H_ */
