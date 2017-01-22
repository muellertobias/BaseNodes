/*
 * CandidateNodeCoreImpl.h
 *
 *  Created on: 21.01.2017
 *      Author: tobias
 */

#ifndef CORE_IMPLEMENTATION_ELECTION_CANDIDATENODECOREIMPL_H_
#define CORE_IMPLEMENTATION_ELECTION_CANDIDATENODECOREIMPL_H_

#include "../../../message/Message.h"
#include "../INodeImpl.h"
#include "ElectionTypes.h"

using network::Message;

namespace core {
namespace implementation {
namespace election {

class CandidateNodeCoreImpl : public INodeImpl {
public:
	CandidateNodeCoreImpl(const int& callPeriod, const Party& party);
	virtual ~CandidateNodeCoreImpl();

	virtual void process(const Message& message);
private:
	const int recallPeriod;
	const Party party;
	int recalls;
};

} /* namespace election */
} /* namespace implementation */
} /* namespace core */

#endif /* CORE_IMPLEMENTATION_ELECTION_CANDIDATENODECOREIMPL_H_ */
