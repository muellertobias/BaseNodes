/*
 * GraphvizNeighborsCreator.h
 *
 *  Created on: 16.11.2016
 *      Author: tobias
 */

#ifndef HELPER_NEIGHBORFINDERS_GRAPHVIZNEIGHBORSCREATOR_H_
#define HELPER_NEIGHBORFINDERS_GRAPHVIZNEIGHBORSCREATOR_H_

#include "AbstractNeighborsCreater.h"
#include "Graph.h"
#include <string>

namespace helper {
namespace neighborFinders {

using namespace std;

class GraphvizNeighborsCreator: public AbstractNeighborsCreater {
public:
	GraphvizNeighborsCreator(const string& filename);
	virtual ~GraphvizNeighborsCreator() { }

	virtual void build(int nodeID, const NodeMap& nodes);
	const virtual NodeMap& getNeighbors(int nodeID) {
		return this->neighbors;
	}
private:
	const string& filename;
	Graph* graph;

	void readFile();
	void readLine(const string& line);
	void readGraph(int nodeID, const NodeMap& nodes);
};

}
} /* namespace helper */

#endif /* HELPER_NEIGHBORFINDERS_GRAPHVIZNEIGHBORSCREATOR_H_ */
