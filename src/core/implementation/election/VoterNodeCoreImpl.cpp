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
		int opponent;
		int* confidenceLevelOfCandidate = 0;
		int* confidenceLevelOfOpponent = 0;
		resolveConfidenceLevels(party, opponent, confidenceLevelOfCandidate, confidenceLevelOfOpponent);

		if (*confidenceLevelOfCandidate < *confidenceLevelOfOpponent) {
			*confidenceLevelOfCandidate -= 1;
			*confidenceLevelOfOpponent += 1;
		} else if (*confidenceLevelOfCandidate > *confidenceLevelOfOpponent) {
			*confidenceLevelOfCandidate += 1;
			*confidenceLevelOfOpponent -= 1;
		}

		limitConfidenceLevel(confidenceLevelOfCandidate);
		limitConfidenceLevel(confidenceLevelOfOpponent);
		//cout << party << "=" << *cl1 << ", " << party2 << "=" << *cl2 << endl;

	} else if (message.getType() == MessageType::application) {
		set<int>::iterator it = receivedMessageNumbers.find(message.getNumber());
		// nur VoteMe akzeptieren, wenn man es nicht schon gehört hat
		if (it != receivedMessageNumbers.end()) {
			return;
		}
		receivedMessageNumbers.insert(message.getNumber());

		int party = stoi(message.getContent());
		int* confidenceLevelOfCandidate = NULL;
		int* confidenceLevelOfOpponent = NULL;
		int opponent = 0;

		resolveConfidenceLevels(party, opponent, confidenceLevelOfCandidate, confidenceLevelOfOpponent);

		(*confidenceLevelOfCandidate) += round(*confidenceLevelOfCandidate / 10);
		if (*confidenceLevelOfCandidate > 100)
			*confidenceLevelOfCandidate = 100;
		(*confidenceLevelOfOpponent) -= round(*confidenceLevelOfCandidate / 10);
		if (*confidenceLevelOfOpponent < 0)
			*confidenceLevelOfOpponent = 0;

		limitConfidenceLevel(confidenceLevelOfCandidate);
		limitConfidenceLevel(confidenceLevelOfOpponent);

		string voting;
		if (*confidenceLevelOfCandidate < *confidenceLevelOfOpponent) {
			voting = "I Dont Vote You";
		} else if (*confidenceLevelOfCandidate > *confidenceLevelOfOpponent) {
			voting = "I Vote You";
			(getCore()->*sendToAll)(message, message.getSourceID());
			// Dies verursacht das Unendliche Senden von VoteMe_messages
			// Abgefangen durch sammeln der MessageNumbers
		}

		//cout << party << "=" << *cl1 << ", " << party2 << "=" << *cl2 << " - " << message.getNumber() << endl;
		if (!voting.empty()) {
			Message replyMsg(MessageType::application, message.getNumber(), voting);
			(getCore()->*sendTo)(replyMsg, (int)party);
		}
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

void VoterNodeCoreImpl::resolveConfidenceLevels(const int& party, int& opponent,
		int* confidenceLevelOfCandidate, int* confidenceLevelOfOpponent) {

	if (party == 1) {
		confidenceLevelOfCandidate = &politics.confidenceLevel1;
		confidenceLevelOfOpponent = &politics.confidenceLevel2;
		opponent = politics.party2;
	} else {
		confidenceLevelOfOpponent = &politics.confidenceLevel2;
		confidenceLevelOfCandidate = &politics.confidenceLevel1;
		opponent = politics.party1;
	}
}

void VoterNodeCoreImpl::limitConfidenceLevel(int* confidenceLevel) {
	if (*confidenceLevel > 100)
		*confidenceLevel = 100;
	else if (*confidenceLevel < 0)
		*confidenceLevel = 0;
}

} /* namespace election */
} /* namespace implementation */
} /* namespace core */
