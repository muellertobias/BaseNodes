/*
 * NodeConfigReader.h
 *
 *  Created on: 27.10.2016
 *      Author: tobias
 */

#ifndef HELPER_SETTINGS_NODEBASESETTINGS_H_
#define HELPER_SETTINGS_NODEBASESETTINGS_H_

#include <string>

#include "../../core/implementation/rumor/RumorNodeCoreImpl.h"
#include "../NodeInfo.h"
#include "Settings.h"

namespace helper {
namespace neighborFinders {
class ISearchNeighbors;
} /* namespace neighborFinders */
} /* namespace helper */

namespace helper {
namespace settings {

using namespace std;
using namespace neighborFinders;

class NodeBaseSettings : public Settings {
public:
	NodeBaseSettings(const string& filename);
	NodeBaseSettings(const string& filename, int nodeID, ISearchNeighbors* neighborSearcher, string config);
	virtual ~NodeBaseSettings();

	NodeInfo getCurrentNodeInfo();
	NodeInfo getNodeInfo(int nodeID);
	NodeMap getNeighbors();

	virtual int getNodeId() const {
		return nodeID;
	}

	virtual core::implementation::INodeImpl* getNodeImplementation() {
		return new core::implementation::rumor::RumorNodeCoreImpl(config);
	}

	virtual network::TransceiverBase* getTransceiver();

protected:
	const int numberOfConnections = 30;

	NodeMap allNodes;
	int nodeID;
	ISearchNeighbors* neighborSearcher;
	int config;

	NodeMap readFile(const string& filename);
	NodeInfo readLine(const string& line);
};

}
} /* namespace helper */

#endif /* HELPER_SETTINGS_NODEBASESETTINGS_H_ */
