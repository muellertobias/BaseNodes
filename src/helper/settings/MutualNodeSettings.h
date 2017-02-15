/*
 * MutualNodeSettings.h
 *
 *  Created on: 14.02.2017
 *      Author: tobias
 */

#ifndef HELPER_SETTINGS_MUTUALNODESETTINGS_H_
#define HELPER_SETTINGS_MUTUALNODESETTINGS_H_

#include "NodeBaseSettings.h"

namespace helper {
namespace settings {

class MutualNodeSettings: public NodeBaseSettings {
public:
	MutualNodeSettings(const string& filename, int nodeID, ISearchNeighbors* neighborSearcher, string config);
	virtual ~MutualNodeSettings();

	virtual core::implementation::INodeImpl* getNodeImplementation();
};

} /* namespace settings */
} /* namespace helper */

#endif /* HELPER_SETTINGS_MUTUALNODESETTINGS_H_ */
