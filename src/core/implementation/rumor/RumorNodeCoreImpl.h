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
#include "../../../network/Message.h"
#include <map>

namespace core {
namespace implementation {
namespace rumor {

typedef struct {
	string Rumor;
	vector<int> Sources;
	unsigned int counter;
} RumorInfo;

typedef pair<string, RumorInfo*> RumorInfoPair;
typedef map<RumorInfoPair::first_type, RumorInfoPair::second_type> RumorInfoMap;

class RumorNodeCoreImpl : public INodeImpl {
public:
	RumorNodeCoreImpl(int threshold);
	RumorNodeCoreImpl(NodeCore* core, int threshold);
	virtual ~RumorNodeCoreImpl();

	virtual void process(const Message& message);

private:
	int threshold;
	RumorInfoMap rumors;
};

} /* namespace rumor */
} /* namespace implementation */
} /* namespace core */

#endif /* CORE_IMPLEMENTATION_RUMOR_RUMORNODECOREIMPL_H_ */
