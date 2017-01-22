/*
 * NodeElectionSettings.h
 *
 *  Created on: 21.01.2017
 *      Author: tobias
 */

#ifndef HELPER_SETTINGS_NODEELECTIONSETTINGS_H_
#define HELPER_SETTINGS_NODEELECTIONSETTINGS_H_

#include "NodeBaseSettings.h"

namespace helper {
namespace settings {

class NodeElectionSettings: public NodeBaseSettings {
public:
	NodeElectionSettings(const string& filename, int nodeID, ISearchNeighbors* neighborSearcher, string config);
	virtual ~NodeElectionSettings();

	virtual core::implementation::INodeImpl* getNodeImplementation();
};

} /* namespace settings */
} /* namespace helper */

#endif /* HELPER_SETTINGS_NODEELECTIONSETTINGS_H_ */
