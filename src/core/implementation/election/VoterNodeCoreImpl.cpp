/*
 * VoterNodeCoreImpl.cpp
 *
 *  Created on: 21.01.2017
 *      Author: tobias
 */

#include "VoterNodeCoreImpl.h"

#include <iostream>
#include <map>

#include "../../NodeCore.h"

namespace core {
namespace implementation {
namespace election {

VoterNodeCoreImpl::VoterNodeCoreImpl(ConfidenceLevels* confidenceLevels)
: confidenceLevels(confidenceLevels) {
}

VoterNodeCoreImpl::~VoterNodeCoreImpl() {
	delete confidenceLevels;
}

void VoterNodeCoreImpl::process(const Message& message) {
	//cout << core->getNodeInfo().NodeID << " : Red=" << confidenceLevels->at(Party::Red) << ", Blue=" << confidenceLevels->at(Party::Blue) << endl;
	if (message.getType() == MessageType::explorer) {
		// Campaign
		Party party = (Party)stoi(message.getContent());
		int cl1 = confidenceLevels->at(party);
		Party party2 = (Party)((party + 1) % 2);
		int cl2 = confidenceLevels->at(party2);
		if (cl1 < cl2) {
			confidenceLevels->at(party) -= 1;
			confidenceLevels->at(party2) += 1;
		} else if (cl1 > cl2) {
			confidenceLevels->at(party) += 1;
			confidenceLevels->at(party2) -= 1;
		}
	} else if (message.getType() == MessageType::application) {

	}
	// Messages to handle
	// - Reset
	// - MessageType::explorer -> Campaign
	// - MessageType::application::VoteMe

	// Messages to send
	// - MessageType::application -> IVoteYou directly to Candidate
	// - MessageType::application -> IDontVoteYou directly to Candidate

}

void VoterNodeCoreImpl::getState(string& state) {
}

} /* namespace election */
} /* namespace implementation */
} /* namespace core */
