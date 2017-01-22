/*
 * RumorNodeCoreImpl.h
 *
 *  Created on: 08.01.2017
 *      Author: tobias
 */

#ifndef CORE_IMPLEMENTATION_RUMOR_RUMORNODECOREIMPL_H_
#define CORE_IMPLEMENTATION_RUMOR_RUMORNODECOREIMPL_H_

#include "../INodeImpl.h"
#include "../../NodeCore.h"
#include "../../../message/Message.h"
#include <map>

namespace core {
namespace implementation {
namespace rumor {

typedef struct {
	string Rumor;
	vector<int> Sources;
	unsigned int counter;
	bool convinced;
} RumorInfo;

typedef pair<string, RumorInfo*> RumorInfoPair;
typedef map<RumorInfoPair::first_type, RumorInfoPair::second_type> RumorInfoMap;

class RumorNodeCoreImpl : public INodeImpl {
public:
	RumorNodeCoreImpl(unsigned int threshold);
	RumorNodeCoreImpl(NodeCore* core, unsigned int threshold);
	virtual ~RumorNodeCoreImpl();

	virtual void process(const Message& message);
	virtual void getState(string& state) {}

	void sendResult(const Message& message);

private:
	unsigned int threshold;
	RumorInfoMap rumors;
};

} /* namespace rumor */
} /* namespace implementation */
} /* namespace core */

#endif /* CORE_IMPLEMENTATION_RUMOR_RUMORNODECOREIMPL_H_ */
