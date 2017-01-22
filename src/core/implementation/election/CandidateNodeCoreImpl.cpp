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
: callPeriod(callPeriod), party(party) {
}

CandidateNodeCoreImpl::~CandidateNodeCoreImpl() {
}

void CandidateNodeCoreImpl::process(const Message& message) {
	cout << core->getNodeInfo().NodeID << " : Callperiod=" <<callPeriod << ", Party=" << party << endl;
}

} /* namespace election */
} /* namespace implementation */
} /* namespace core */
