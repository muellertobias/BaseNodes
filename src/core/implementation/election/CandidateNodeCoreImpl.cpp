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

// Messages to handle
// - Init
// - Reset
// - MessageType::echo -> Campaign erfolgreich
// - MessageType::application -> IVoteYou
// - MessageType::application -> IDontVoteYou

// Messages to send
// - MessageType::explorer -> Campaign
// - MessageType::application -> VoteMe

CandidateNodeCoreImpl::CandidateNodeCoreImpl(const int& callPeriod, const int& party)
: recallPeriod(callPeriod), party(party), recalls(0) {
}

CandidateNodeCoreImpl::~CandidateNodeCoreImpl() {
}

void CandidateNodeCoreImpl::process(Message* const message) {
	if (dynamic_cast<ControlMessage*>(message) != NULL) {
		process((ControlMessage*)message);
	} else if (dynamic_cast<ApplicationMessage*>(message) != NULL) {
		process((ApplicationMessage*)message);
	}
}

void CandidateNodeCoreImpl::process(ControlMessage* const message) {
	// Reset or Init
	if (message->getType() == MessageSubType::normal) {
		cout << party << " - Init!" << endl;
		startCampaign();
	}
}

void CandidateNodeCoreImpl::process(ApplicationMessage* const message) {
	try {
		if (message->getType() == MessageSubType::explorer) {
			// Campaign des Gegners!
			cout << party << " - Campaign from Oppenent detected!" << endl;
		} else if (message->getType() == MessageSubType::echo) {
			// Die Antworten der eigenen Campaign
			cout << party << " - Campaign successful!" << endl;

		} else if (message->getType() == MessageSubType::normal) {
			// Voter's Choice
			cout << party << " - " << message->getSourceID() << ": " << message->getContent() << endl;
		}
		recalls++;
		if (recalls % recallPeriod == 1) {
			// starte Campaign oder VoteMe
			int strategy = randomizer::random(0, 1);
			if (strategy == 0) {
				callVoteMe();
			} else {
				startCampaign(); // Chance für Kampagne erhöht, wegen der Verteilung der Vectorzeit
			}
		}
	} catch (std::exception& ex) {
		helper::utilities::writeLog(__FUNCTION__, ex);
	}
}

void CandidateNodeCoreImpl::getState(string& state) {
}

void CandidateNodeCoreImpl::startCampaign() {
	cout << party << " - Start Campaign!" << endl;
	(getCore()->*sendEcho)(to_string(party));
}

void CandidateNodeCoreImpl::callVoteMe() {
	cout << party << " - Start VoteMe!" << endl;
	ApplicationMessage* voteMeMsg = new ApplicationMessage(MessageSubType::normal, to_string(party));
	(getCore()->*sendToAll)(voteMeMsg, 0);
}

} /* namespace election */
} /* namespace implementation */
} /* namespace core */
