/*
 * Graph.cpp
 *
 *  Created on: 16.11.2016
 *      Author: tobias
 */

#include "Graph.h"

namespace helper {
namespace neighborFinders {

Graph::Graph() {
	this->graphMap = new GraphMap();
}

Graph::~Graph() {
	for (GraphMap::iterator it = graphMap->begin(); it != graphMap->end(); ++it) {
		(*it).second->clear();
		delete (*it).second;
	}

	graphMap->clear();
	delete this->graphMap;
}

bool Graph::addEdge(int firstNodeID, int secondNodeID) const {
	insert(firstNodeID, secondNodeID);
	insert(secondNodeID, firstNodeID);
	return true;
}

NeighborList* Graph::getNeighbors(int nodeID) {
	return graphMap->at(nodeID);
}

void Graph::insert(int firstNodeID, int secondNodeID) const {
	if (graphMap->empty() || graphMap->count(firstNodeID) == 0) {
		NeighborList* neighborList = new NeighborList();
		neighborList->push_back(secondNodeID);
		graphMap->insert(GraphPair(firstNodeID, neighborList));
	} else {
		graphMap->at(firstNodeID)->push_back(secondNodeID);
	}
}

}
} /* namespace helper */
