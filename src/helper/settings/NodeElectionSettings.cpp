/*
 * NodeElectionSettings.cpp
 *
 *  Created on: 21.01.2017
 *      Author: tobias
 */

#include "NodeElectionSettings.h"

#include <map>
#include <string>
#include <utility>
#include <iostream>

#include "../../core/implementation/election/CandidateNodeCoreImpl.h"
#include "../../core/implementation/election/ElectionTypes.h"
#include "../../core/implementation/election/VoterNodeCoreImpl.h"
#include "../neighborFinders/ISearchNeighbors.h"
#include "../NodeInfo.h"
#include "../randomizer/Random.h"


namespace helper {
namespace settings {

NodeElectionSettings::NodeElectionSettings(const string& filename, int nodeID,
		ISearchNeighbors* neighborSearcher, string config) : NodeBaseSettings(filename, nodeID, neighborSearcher, config) {
}

NodeElectionSettings::~NodeElectionSettings() {

}

core::implementation::INodeImpl* NodeElectionSettings::getNodeImplementation() {
	using namespace core::implementation::election;
	int biggestNodeID = this->allNodes.size() - 1;
	//cout << biggestNodeID << endl;
	if (this->nodeID == 1 || this->nodeID == biggestNodeID) {
		// Kandidaten
		return new CandidateNodeCoreImpl(config, this->nodeID);
	} else {
		// Wähler
		Politics politics = {1, 0, biggestNodeID, 0};

		const NodeMap& neighbors = this->neighborSearcher->getNeighbors(this->nodeID);
		if (neighbors.find(1) != neighbors.end()) {
			// Parteifreund von 1
			politics.confidenceLevel1 = 100;
		} else if (neighbors.find(biggestNodeID) != neighbors.end()) {
			// Parteifreund von n
			politics.confidenceLevel2 = 100;
		} else {
			// Normaler Wähler
			politics.confidenceLevel1 = randomizer::random(0, 100);;
			politics.confidenceLevel2 = randomizer::random(0, 100);;
		}

		return new VoterNodeCoreImpl(politics);
	}
}

} /* namespace settings */
} /* namespace helper */
