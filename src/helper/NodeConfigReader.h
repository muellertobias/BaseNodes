/*
 * NodeConfigReader.h
 *
 *  Created on: 27.10.2016
 *      Author: tobias
 */

#ifndef NODECONFIGREADER_H_
#define NODECONFIGREADER_H_

#include "../NodeInfo.h"
#include "interfaces/IConfigureNode.h"
#include "neighborFinders/ISearchNeighbors.h"

namespace helper {

using namespace std;
using namespace neighborFinders;
using namespace interfaces;

class NodeConfigReader : public IConfigureNode {
public:
	NodeConfigReader(const string& filename, int nodeID, ISearchNeighbors* neighborSearcher);
	virtual ~NodeConfigReader();

	NodeInfo getCurrentNodeInfo();
	NodeInfo getNodeInfo(int nodeID);
	NodeMap getNeighbors();

private:
	NodeMap allNodes;
	int nodeID;
	ISearchNeighbors* neighborSearcher;

	NodeMap readFile(const string& filename);
	NodeInfo readLine(const string& line);
};

} /* namespace helper */

#endif /* NODECONFIGREADER_H_ */
