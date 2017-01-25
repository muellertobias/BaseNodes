/*
 * VoterNodeCoreImpl.h
 *
 *  Created on: 21.01.2017
 *      Author: tobias
 */

#ifndef CORE_IMPLEMENTATION_ELECTION_VOTERNODECOREIMPL_H_
#define CORE_IMPLEMENTATION_ELECTION_VOTERNODECOREIMPL_H_

#include "../../../message/Message.h"
#include "../INodeImpl.h"
#include "ElectionTypes.h"
#include <set>

using namespace message;

namespace core {
namespace implementation {
namespace election {

class VoterNodeCoreImpl : public INodeImpl {
public:
	VoterNodeCoreImpl(const Politics& politics);
	virtual ~VoterNodeCoreImpl();

	virtual void process(const Message& message);
	virtual void getState(string& state);

private:
	Politics politics;
	set<int> receivedMessageNumbers;

	void resolveConfidenceLevels(const int& party, int& opponent, int* confidenceLevelOfCandidate, int* confidenceLevelOfOpponent);
	void limitConfidenceLevel(int* confidenceLevel);
};

} /* namespace election */
} /* namespace implementation */
} /* namespace core */

#endif /* CORE_IMPLEMENTATION_ELECTION_VOTERNODECOREIMPL_H_ */
