/*
 * NodeCoreBaseImpl.h
 *
 *  Created on: 26.11.2016
 *      Author: tobias
 */

#ifndef CORE_IMPLEMENTATION_NODECOREBASEIMPL_H_
#define CORE_IMPLEMENTATION_NODECOREBASEIMPL_H_

#include "../../network/Message.h"
#include "INodeImpl.h"
#include "../data/MessageMap.h"
#include <set>
#include <tuple>

namespace core {
namespace implementation {


typedef tuple<MessageSource, MessageNumber> MinimalMessageInfoTupel;
typedef set<MinimalMessageInfoTupel> MinimalMessageInfoSet;

class NodeCoreBaseImpl : public INodeImpl {
public:
	NodeCoreBaseImpl(NodeCore* core);
	virtual ~NodeCoreBaseImpl() { };
	virtual void process(const Message& message);

	virtual void setSendToDestinations(NodeCore::_sendToAll sendToAll) {
		this->sendToAll = sendToAll;
	}

private:
	NodeCore* core;
	NodeCore::_sendToAll sendToAll;
	MinimalMessageInfoSet messageSet;
};

} /* namespace implementation */
} /* namespace core */

#endif /* CORE_IMPLEMENTATION_NODECOREBASEIMPL_H_ */
