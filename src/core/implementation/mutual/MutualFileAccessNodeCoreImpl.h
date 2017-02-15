/*
 * MutualFileAccessNodeCoreImpl.h
 *
 *  Created on: 14.02.2017
 *      Author: tobias
 */

#ifndef CORE_IMPLEMENTATION_MUTUAL_MUTUALFILEACCESSNODECOREIMPL_H_
#define CORE_IMPLEMENTATION_MUTUAL_MUTUALFILEACCESSNODECOREIMPL_H_

#include <string>

#include "../INodeImpl.h"
#include "RequestQueue.h"

namespace core {
namespace implementation {
namespace mutual {

class MutualFileAccessNodeCoreImpl : public INodeImpl {
public:
	MutualFileAccessNodeCoreImpl(int highestNodeID);
	virtual ~MutualFileAccessNodeCoreImpl();

	virtual void process(Message* const message);
	virtual INodeImpl* prototype() { return this; };
	virtual void getState(string& state);

private:
	RequestQueue* queue;
	int terminiationCounter;
	int highestNodeID;

	void criticalSection();
};

} /* namespace mutual */
} /* namespace implementation */
} /* namespace core */

#endif /* CORE_IMPLEMENTATION_MUTUAL_MUTUALFILEACCESSNODECOREIMPL_H_ */
