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

using network::Message;

namespace core {
namespace implementation {
namespace election {

class VoterNodeCoreImpl : public INodeImpl {
public:
	VoterNodeCoreImpl();
	virtual ~VoterNodeCoreImpl();

	virtual void process(const Message& message);

};

} /* namespace election */
} /* namespace implementation */
} /* namespace core */

#endif /* CORE_IMPLEMENTATION_ELECTION_VOTERNODECOREIMPL_H_ */
