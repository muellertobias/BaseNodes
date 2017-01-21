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

using network::Message;

namespace core {
namespace implementation {
namespace election {

class CandidateNodeCoreImpl : public INodeImpl {
public:
	CandidateNodeCoreImpl();
	virtual ~CandidateNodeCoreImpl();

	virtual void process(const Message& message);

};

} /* namespace election */
} /* namespace implementation */
} /* namespace core */

#endif /* CORE_IMPLEMENTATION_ELECTION_CANDIDATENODECOREIMPL_H_ */
