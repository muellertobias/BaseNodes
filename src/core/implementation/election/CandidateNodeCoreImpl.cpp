/*
 * CandidateNodeCoreImpl.cpp
 *
 *  Created on: 21.01.2017
 *      Author: tobias
 */

#include "CandidateNodeCoreImpl.h"

namespace core {
namespace implementation {
namespace election {

CandidateNodeCoreImpl::CandidateNodeCoreImpl(const int& callPeriod, const Party& party)
: recallPeriod(callPeriod), party(party), recalls(0) {
}

CandidateNodeCoreImpl::~CandidateNodeCoreImpl() {
}

void CandidateNodeCoreImpl::process(const Message& message) {
	//cout << core->getNodeInfo().NodeID << " : Callperiod=" << recallPeriod << ", current Recalls= " << recalls << ", Party=" << party << endl;

	// Messages to handle
	// - Init
	// - Reset
	// - MessageType::echo -> Auflistung aller Stimmen und Gegenstimmen
	// - MessageType::application -> IVoteYou
	// - MessageType::application -> IDontVoteYou

	// Messages to send
	// - MessageType::explorer -> Campaign
	// - MessageType::application -> VoteMe
}

void CandidateNodeCoreImpl::getState(string& state) {
}

} /* namespace election */
} /* namespace implementation */
} /* namespace core */
