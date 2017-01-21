/*
 * NodeConfigReader.h
 *
 *  Created on: 27.10.2016
 *      Author: tobias
 */

#ifndef HELPER_SETTINGS_NODEBASESETTINGS_H_
#define HELPER_SETTINGS_NODEBASESETTINGS_H_

#include <string>

#include "../../helper/NodeInfo.h"
#include "../settings/Settings.h"

namespace helper {
namespace neighborFinders {
class ISearchNeighbors;
} /* namespace neighborFinders */
} /* namespace helper */

namespace helper {
namespace settings {

using namespace std;
using namespace neighborFinders;

class NodeBaseSettings : public Settings {
public:
	NodeBaseSettings(const string& filename);
	NodeBaseSettings(const string& filename, int nodeID, ISearchNeighbors* neighborSearcher);
	virtual ~NodeBaseSettings();

	NodeInfo getCurrentNodeInfo();
	NodeInfo getNodeInfo(int nodeID);
	NodeMap getNeighbors();

	virtual int getNodeId() const {
		return nodeID;
	}

protected:
	NodeMap allNodes;
	int nodeID;
	ISearchNeighbors* neighborSearcher;

	NodeMap readFile(const string& filename);
	NodeInfo readLine(const string& line);
};

}
} /* namespace helper */

#endif /* HELPER_SETTINGS_NODEBASESETTINGS_H_ */
