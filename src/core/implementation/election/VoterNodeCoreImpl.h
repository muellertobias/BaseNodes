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

using network::Message;

namespace core {
namespace implementation {
namespace election {

class VoterNodeCoreImpl : public INodeImpl {
public:
	VoterNodeCoreImpl(ConfidenceLevels* confidenceLevels);
	virtual ~VoterNodeCoreImpl();

	virtual void process(const Message& message);
	virtual void getState(string& state);

private:
	ConfidenceLevels* confidenceLevels;
};

} /* namespace election */
} /* namespace implementation */
} /* namespace core */

#endif /* CORE_IMPLEMENTATION_ELECTION_VOTERNODECOREIMPL_H_ */
