/*
 * VoterNodeCoreImpl.cpp
 *
 *  Created on: 21.01.2017
 *      Author: tobias
 */

#include "VoterNodeCoreImpl.h"

#include <iostream>
#include <map>
#include <cmath>

#include "../../../helper/utilities/utilities.h"
#include "../../NodeCore.h"

namespace core {
namespace implementation {
namespace election {

VoterNodeCoreImpl::VoterNodeCoreImpl(const Politics& politics)
: politics(politics) {
}

VoterNodeCoreImpl::~VoterNodeCoreImpl() {
}

void VoterNodeCoreImpl::process(const Message& message) {
	//cout << core->getNodeInfo().NodeID << " : Red=" << confidenceLevels->at(Party::Red) << ", Blue=" << confidenceLevels->at(Party::Blue) << endl;
	if (message.getType() == MessageType::explorer) {
		// Campaign
		if (!helper::utilities::isNumber(message.getContent())) {
			helper::utilities::writeLog(message.getContent() + " is not a number! - ignored message");
			return;
		}

		int party = stoi(message.getContent());
		int* cl1;
		int* cl2;
		int party2;
		if (party == 1) {
			cl1 = &politics.confidenceLevel1;
			cl2 = &politics.confidenceLevel2;
			party2 = politics.party2;
		} else {
			cl1 = &politics.confidenceLevel2;
			cl2 = &politics.confidenceLevel1;
			party2 = politics.party1;
		}

		if (*cl1 < *cl2) {
			*cl1 -= 1;
			*cl2 += 1;
		} else if (*cl1 > *cl2) {
			*cl1 += 1;
			*cl2 -= 1;
		}
		if (*cl1 > 100)
			*cl1 = 100;
		else if (*cl1 < 0)
			*cl1 = 0;

		if (*cl2 > 100)
			*cl2 = 100;
		else if (*cl2 < 0)
			*cl2 = 0;

		//cout << party << "=" << *cl1 << ", " << party2 << "=" << *cl2 << endl;

	} else if (message.getType() == MessageType::application) {
		set<int>::iterator it = receivedMessageNumbers.find(message.getNumber());
		// nur VoteMe akzeptieren, wenn man es nicht schon gehört hat
		if (it != receivedMessageNumbers.end()) {
			return;
		}
		int party = stoi(message.getContent());
		int* cl1;
		int* cl2;
		int party2;
		if (party == 1) {
			cl1 = &politics.confidenceLevel1;
			cl2 = &politics.confidenceLevel2;
			party2 = politics.party2;
		} else {
			cl1 = &politics.confidenceLevel2;
			cl2 = &politics.confidenceLevel1;
			party2 = politics.party1;
		}

		(*cl1) += round(*cl1 / 10);
		if (*cl1 > 100)
			*cl1 = 100;
		(*cl2) -= round(*cl1 / 10);
		if (*cl2 < 0)
			*cl2 = 0;

		string voting;
		if (*cl1 < *cl2) {
			voting = "I Dont Vote You";
		} else if (*cl1 > *cl2) {
			voting = "I Vote You";
			(getCore()->*sendToAll)(message, message.getSourceID());
			// Dies verursacht das Unendliche Senden von VoteMe_messages
			// Abgefangen durch sammeln der MessageNumbers
		}

		//cout << party << "=" << *cl1 << ", " << party2 << "=" << *cl2 << " - " << message.getNumber() << endl;


		Message replyMsg(MessageType::application, message.getNumber(), voting);
		(getCore()->*sendTo)(replyMsg, (int)party);
		receivedMessageNumbers.insert(message.getNumber());
	}
	// Messages to handle
	// - Reset
	// - MessageType::explorer -> Campaign content=NodeID von C
	// - MessageType::application -> VoteMe content=NodeID von C

	// Messages to send
	// - MessageType::application -> IVoteYou directly to Candidate
	// - MessageType::application -> IDontVoteYou directly to Candidate

}

void VoterNodeCoreImpl::getState(string& state) {
}

} /* namespace election */
} /* namespace implementation */
} /* namespace core */
