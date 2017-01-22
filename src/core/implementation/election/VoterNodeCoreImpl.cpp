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
	cout << core->getNodeInfo().NodeID << " : Red=" << confidenceLevels->at(Party::Red) << ", Blue=" << confidenceLevels->at(Party::Blue) << endl;
}

} /* namespace election */
} /* namespace implementation */
} /* namespace core */
