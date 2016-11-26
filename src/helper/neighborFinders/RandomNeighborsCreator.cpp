/*
 * RandomNeighborsCreator.cpp
 *
 *  Created on: 30.10.2016
 *      Author: tobias
 */

#include "RandomNeighborsCreator.h"

#include <stddef.h>
#include <cstdio>
#include <iterator>
#include <map>
#include <random>
#include <utility>

namespace helper {
namespace neighborFinders {

RandomNeighborsCreator::RandomNeighborsCreator(int requestedNumberOfNeighbors)
	: requestedNumberOfNeighbors(requestedNumberOfNeighbors) {
}

RandomNeighborsCreator::~RandomNeighborsCreator() {
}

void RandomNeighborsCreator::build(int nodeID, const NodeMap& nodes) {
	// zufällig 3 wählen und speichern
	NodeMap neighbors(nodes);
	neighbors.erase(nodeID);

	if (neighbors.size() <= (size_t)requestedNumberOfNeighbors) {
		this->neighbors = neighbors;
	} else {
		size_t numberOfNeighbors = requestedNumberOfNeighbors;

		std::random_device generator;

		while (numberOfNeighbors > 0) {
			auto element = neighbors.begin();
			std::advance(element, generator() % neighbors.end()->first);

			auto elementAlreadyExists = this->neighbors.insert(NodePair(element->first, element->second));
			if (elementAlreadyExists.second) {
				--numberOfNeighbors;
			}
		}
	}
}

const NodeMap& RandomNeighborsCreator::getNeighbors(int nodeID) {
	return neighbors;
}

}
} /* namespace helper */
