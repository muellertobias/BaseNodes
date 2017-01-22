/*
 * Initiator.h
 *
 *  Created on: 11.11.2016
 *      Author: tobias
 */

#ifndef INITIATOR_INITIATOR_H_
#define INITIATOR_INITIATOR_H_

#include <string>

#include "../core/NodeCore.h"
#include "../message/Message.h"
#include "../helper/NodeInfo.h"

using namespace core;
using namespace std;

class Initiator {
public:
	Initiator(const NodeCore& core);
	virtual ~Initiator() { }

	void loop();
private:
	const NodeCore& core;
	const NodeMap& nodes;

	void showDetails();
	string readInput() const;
	void tell();
	void halt();
	void echo();
	void snapshoot();
	bool sendTo(const string& content, const NodeInfo& nodeID, int sourceID);
	bool sendTo(const Message& message, const NodeInfo& nodeID);
};


#endif /* INITIATOR_INITIATOR_H_ */
