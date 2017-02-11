/*
 * VoterNodeCoreImpl.cpp
 *
 *  Created on: 21.01.2017
 *      Author: tobias
 */

#include "VoterNodeCoreImpl.h"

#include <iostream>
#include <sstream>
#include <map>
#include <cmath>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

#include "../../../helper/utilities/utilities.h"
#include "../../NodeCore.h"

namespace core {
namespace implementation {
namespace election {

// Messages to handle
// - Reset
// - MessageType::explorer -> Campaign content=NodeID von C
// - MessageType::application -> VoteMe content=NodeID von C

// Messages to send
// - MessageType::application -> IVoteYou directly to Candidate
// - MessageType::application -> IDontVoteYou directly to Candidate

VoterNodeCoreImpl::VoterNodeCoreImpl(const Politics& politics)
: politics(politics) {
}

VoterNodeCoreImpl::~VoterNodeCoreImpl() {
}


void VoterNodeCoreImpl::process(Message* const message) {
	if (dynamic_cast<ControlMessage*>(message) != NULL) {
		process((ControlMessage*)message);
	} else if (dynamic_cast<ApplicationMessage*>(message) != NULL) {
		process((ApplicationMessage*)message);
	}
}

void VoterNodeCoreImpl::process(ControlMessage* const message) {
}

void VoterNodeCoreImpl::process(ApplicationMessage* const message) {
    std::this_thread::sleep_for (std::chrono::microseconds(700));

	if (message->getType() == MessageSubType::explorer) {
		// Campaign
		if (!helper::utilities::isNumber(message->getContent())) {
			helper::utilities::writeLog(message->getContent() + " is not a number! - ignored message");
			return;
		}

		int party = stoi(message->getContent());
		int* confidenceLevelOfCandidate = NULL;
		int* confidenceLevelOfOpponent = NULL;
		if (party == 1) {
			confidenceLevelOfCandidate = &politics.confidenceLevel1;
			confidenceLevelOfOpponent = &politics.confidenceLevel2;
		} else {
			confidenceLevelOfOpponent = &politics.confidenceLevel2;
			confidenceLevelOfCandidate = &politics.confidenceLevel1;
		}

		if (*confidenceLevelOfCandidate < *confidenceLevelOfOpponent) {
			*confidenceLevelOfCandidate -= 1;
			*confidenceLevelOfOpponent += 1;
		} else if (*confidenceLevelOfCandidate > *confidenceLevelOfOpponent) {
			*confidenceLevelOfCandidate += 1;
			*confidenceLevelOfOpponent -= 1;
		}

		limitConfidenceLevel(confidenceLevelOfCandidate);
		limitConfidenceLevel(confidenceLevelOfOpponent);

	} else if (message->getType() == MessageSubType::normal) {
		set<int>::iterator it = receivedMessageNumbers.find(message->getNumber());
		// nur VoteMe akzeptieren, wenn man es nicht schon gehört hat
		if (it != receivedMessageNumbers.end()) {
			return;
		}
		receivedMessageNumbers.insert(message->getNumber());

		int party = stoi(message->getContent());
		int* confidenceLevelOfCandidate = NULL;
		int* confidenceLevelOfOpponent = NULL;

		if (party == 1) {
			confidenceLevelOfCandidate = &politics.confidenceLevel1;
			confidenceLevelOfOpponent = &politics.confidenceLevel2;
		} else {
			confidenceLevelOfCandidate = &politics.confidenceLevel2;
			confidenceLevelOfOpponent = &politics.confidenceLevel1;
		}

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
			sendToAll(message, message->getSourceID());
		}

		if (!voting.empty()) {
			ApplicationMessage* replyToCandidate = new ApplicationMessage(MessageSubType::normal, message->getNumber(), voting);
			sendTo(replyToCandidate, (int)party);
		}
	}
}

void VoterNodeCoreImpl::getState(string& state) {
	stringstream strStream;
	strStream << "Voter:" << politics.confidenceLevel1 << "|" << politics.confidenceLevel2;
	state.append(strStream.str());
}

INodeImpl* VoterNodeCoreImpl::prototype() {
	INodeImpl* impl = new VoterNodeCoreImpl(*this);
	return impl;
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
