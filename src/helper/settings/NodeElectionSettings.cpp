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
	int sizeOfGraph = this->allNodes.size() - 1;
	cout << sizeOfGraph << endl;
	if (this->nodeID == 1 || this->nodeID == sizeOfGraph) {
		// Kandidaten
		Party party = this->nodeID == 1 ? Party::Red : Party::Blue;
		cout << party << endl;
		return new CandidateNodeCoreImpl(config, party);
	} else {
		// Wähler
		core::implementation::election::ConfidenceLevels* confidenceLevels = new ConfidenceLevels();
		confidenceLevels->insert(pair<Party,int>(Party::Red, 0));
		confidenceLevels->insert(pair<Party,int>(Party::Blue, 0));

		const NodeMap& neighbors = this->neighborSearcher->getNeighbors(this->nodeID);
		if (neighbors.find(1) != neighbors.end()) {
			// Parteifreund von 1
			confidenceLevels->at(Party::Red) = 100;
		} else if (neighbors.find(sizeOfGraph) != neighbors.end()) {
			// Parteifreund von n
			confidenceLevels->at(Party::Blue) = 100;
		} else {
			// Normaler Wähler
			confidenceLevels->at(Party::Red) = randomizer::random(0, 100);
			confidenceLevels->at(Party::Blue) = randomizer::random(0, 100);
		}

		return new VoterNodeCoreImpl(confidenceLevels);
	}
}

} /* namespace settings */
} /* namespace helper */
