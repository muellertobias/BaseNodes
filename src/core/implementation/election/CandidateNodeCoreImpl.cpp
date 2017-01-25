/*
 * CandidateNodeCoreImpl.cpp
 *
 *  Created on: 21.01.2017
 *      Author: tobias
 */

#include "CandidateNodeCoreImpl.h"

#include <exception>
#include <iostream>
#include <string>

#include "../../../helper/randomizer/Random.h"
#include "../../../helper/utilities/utilities.h"
#include "../../NodeCore.h"

namespace core {
namespace implementation {
namespace election {

CandidateNodeCoreImpl::CandidateNodeCoreImpl(const int& callPeriod, const int& party)
: recallPeriod(callPeriod), party(party), recalls(0) {
}

CandidateNodeCoreImpl::~CandidateNodeCoreImpl() {
}

void CandidateNodeCoreImpl::process(const Message& message) {
	try {
		if (message.getType() == MessageType::control) {
			// Reset or Init
		} else {
			if (message.getType() == MessageType::explorer) {
				// Campaign des Gegners!
			} else if (message.getType() == MessageType::echo) {
				// Die Antworten der eigenen Campaign
				cout << party << " - " << "Campaign war erfolgreich!" << endl;

			} else if (message.getType() == MessageType::application) {
				// Voter's Choice
				cout << party << " - " << message.getSourceID() << ": " << message.getContent() << endl;
			}
			recalls++;
			if (recalls % recallPeriod == 1) {
				// starte Campaign oder VoteMe
				int strategy = randomizer::random(0, 1);
				if (strategy == 0) {
					cout << party << " - " << "Start Campaign!" << endl;
					startCampaign();
				} else {
					cout << party << " - " << "Start VoteMe!" << endl;
					callVoteMe();
				}
			}
		}
	} catch (std::exception& ex) {
		helper::utilities::writeLog(__FUNCTION__, ex);
	}


	// Messages to handle
	// - Init
	// - Reset
	// - MessageType::echo -> Campaign erfolgreich
	// - MessageType::application -> IVoteYou
	// - MessageType::application -> IDontVoteYou

	// Messages to send
	// - MessageType::explorer -> Campaign
	// - MessageType::application -> VoteMe
}

void CandidateNodeCoreImpl::getState(string& state) {
}

void CandidateNodeCoreImpl::startCampaign() {
	(getCore()->*sendEcho)(to_string(party));
}

void CandidateNodeCoreImpl::callVoteMe() {
	Message voteMe(MessageType::application, to_string(party));
	(getCore()->*sendToAll)(voteMe, 0);
}

} /* namespace election */
} /* namespace implementation */
} /* namespace core */
