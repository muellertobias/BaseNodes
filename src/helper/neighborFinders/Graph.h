/*
 * Graph.h
 *
 *  Created on: 16.11.2016
 *      Author: tobias
 */

#ifndef HELPER_NEIGHBORFINDERS_GRAPH_H_
#define HELPER_NEIGHBORFINDERS_GRAPH_H_

#include <vector>
#include <map>

using namespace std;

namespace helper {
namespace neighborFinders {

typedef vector<int> NeighborList;
typedef pair<int, NeighborList*> GraphPair;
typedef map<GraphPair::first_type, GraphPair::second_type> GraphMap;

class Graph {
public:
	Graph();
	virtual ~Graph();

	bool addEdge(int firstNodeID, int secondNodeID) const;
	NeighborList* getNeighbors(int nodeID);

private:
	GraphMap* graphMap;

	void insert(int firstNodeID, int secondNodeID) const;
};

}
} /* namespace helper */

#endif /* HELPER_NEIGHBORFINDERS_GRAPH_H_ */
