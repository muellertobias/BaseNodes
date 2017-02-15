/*
 * MutualNodeSettings.cpp
 *
 *  Created on: 14.02.2017
 *      Author: tobias
 */

#include "MutualNodeSettings.h"
#include <iostream>

#include "../../core/implementation/mutual/MutualFileAccessNodeCoreImpl.h"

namespace helper {
namespace settings {

MutualNodeSettings::MutualNodeSettings(const string& filename, int nodeID, ISearchNeighbors* neighborSearcher, string config)
: NodeBaseSettings(filename, nodeID, neighborSearcher, config) {
	// TODO Auto-generated constructor stub

}

MutualNodeSettings::~MutualNodeSettings() {
	// TODO Auto-generated destructor stub
}

core::implementation::INodeImpl* MutualNodeSettings::getNodeImplementation() {
	int biggestNodeID = this->allNodes.size() - 1;
	return new core::implementation::mutual::MutualFileAccessNodeCoreImpl(biggestNodeID);
}

} /* namespace settings */
} /* namespace helper */
