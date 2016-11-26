/*
 * ExplicitNeighborsCreater.cpp
 *
 *  Created on: 09.11.2016
 *      Author: tobias
 */

#include "ExplicitNeighborsCreater.h"

#include "../exception/ConfigurationException.h"
#include <vector>
#include <map>

namespace helper {
namespace neighborFinders {

ExplicitNeighborsCreater::ExplicitNeighborsCreater(const std::vector<int>& nodeIDs)
	: nodeIDs(nodeIDs) {
}

ExplicitNeighborsCreater::~ExplicitNeighborsCreater() {
}

void ExplicitNeighborsCreater::build(int nodeID, const NodeMap& nodes) {
	if (nodeID == -1) {
		this->neighbors.insert(nodes.begin(), nodes.end());
		return;
	}

	// aus NodeMap nodes die Daten der benachbarten Knoten ermitteln
	for (size_t i = 0; i < nodeIDs.size(); ++i) {
		int nodeID = nodeIDs[i];

		// hier kann eine out_of_range-Exception geworfen werden,
		// wenn die übergebene NodeID nicht in der Konfigurationsdatei existierte.
		try {
			NodeInfo pair = nodes.at(nodeID);
			this->neighbors.insert(NodePair(nodeID, pair));
		} catch (const std::out_of_range& ex) {
			throw new exception::ConfigurationException("Unexpected NodeID: " + nodeID);
		}

	}
}
const NodeMap& ExplicitNeighborsCreater::getNeighbors(int nodeID) {
	return this->neighbors;
}

}
} /* namespace helper */
