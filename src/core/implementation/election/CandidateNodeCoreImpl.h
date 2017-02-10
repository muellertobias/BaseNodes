/*
 * CandidateNodeCoreImpl.h
 *
 *  Created on: 21.01.2017
 *      Author: tobias
 */

#ifndef CORE_IMPLEMENTATION_ELECTION_CANDIDATENODECOREIMPL_H_
#define CORE_IMPLEMENTATION_ELECTION_CANDIDATENODECOREIMPL_H_

#include "../../../message/ApplicationMessage.h"
#include "../../../message/ControlMessage.h"
#include "../INodeImpl.h"
#include "ElectionTypes.h"

using namespace message;


namespace core {
namespace implementation {
namespace election {

class CandidateNodeCoreImpl : public INodeImpl {
public:
	CandidateNodeCoreImpl(const int& callPeriod, const int& party);
	virtual ~CandidateNodeCoreImpl();

	virtual void process(Message* const message);
	virtual void getState(string& state);

	virtual INodeImpl* prototype();

private:
	const int recallPeriod;
	const int party;
	int recalls;

	void process(ControlMessage* const message);
	void process(ApplicationMessage* const message);

	void startCampaign();
	void callVoteMe();
};

} /* namespace election */
} /* namespace implementation */
} /* namespace core */

#endif /* CORE_IMPLEMENTATION_ELECTION_CANDIDATENODECOREIMPL_H_ */
