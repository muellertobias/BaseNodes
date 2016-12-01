/*
 * GraphvizNeighborsCreator.cpp
 *
 *  Created on: 16.11.2016
 *      Author: tobias
 */

#include "GraphvizNeighborsCreator.h"

#include <stddef.h>
#include <fstream>
#include <initializer_list>
#include <iterator>
#include <map>
#include <utility>
#include <algorithm>
#include <vector>

#include "../../NodeInfo.h"
#include "../string/trim.h"

namespace helper {
namespace neighborFinders {

GraphvizNeighborsCreator::GraphvizNeighborsCreator(const string& filename)
	: filename(filename) {
	this->graph = new Graph();
}



void GraphvizNeighborsCreator::build(int nodeID, const NodeMap& nodes) {
	readFile();

	if (nodeID == -1) {
		// Alle in der Adressdatei eingetragenen Nodes werden dem Initiator angezeigt
		this->neighbors = nodes;
	} else {
		readGraph(nodeID, nodes);
	}
}

void GraphvizNeighborsCreator::readGraph(int nodeID, const NodeMap& nodes) {
	NeighborList* neighborList = graph->getNeighbors(nodeID);
	for (size_t i = 0; i < neighborList->size(); ++i) {
		int neighborID = neighborList->at(i);
		NodeInfo neighborInfo = nodes.at(neighborID);
		this->neighbors.insert(NodePair(neighborID, neighborInfo));
	}
}

void GraphvizNeighborsCreator::readFile() {
	string line;
	ifstream file(filename);
	if (file.is_open()) {
		while (getline(file, line)) {
			if (!line.empty()) {
				// Zeile sieht entweder so aus "graph G {", "}" oder "x -- y;" aus
				if (line.find(';') != string::npos) {
					readLine(line);
				}
			}
		}
		file.close();
	}
}

// TODO: Verkettung möglich machen 1 -- 2 -- 3;
void GraphvizNeighborsCreator::readLine(const string& line) {
	vector<string> splittedLine = split(line, ";");

	for (vector<string>::const_iterator it = splittedLine.begin(); it != splittedLine.end(); ++it) {
		if (!(*it).empty()) {
			vector<string> edges = split((*it), "--");
			int firstNodeID = stoi(edges.at(0));
			int secondNodeID = stoi(edges.at(1));
			graph->addEdge(firstNodeID, secondNodeID);
		}
	}
}

}
} /* namespace helper */
