/*
 * IConfigureNode.h
 *
 *  Created on: 30.10.2016
 *      Author: tobias
 */

#ifndef HELPER_SETTINGS_SETTINGS_H_
#define HELPER_SETTINGS_SETTINGS_H_


namespace helper {
namespace settings {

const int ListenerNodeID = 0;

class Settings {
public:
	virtual ~Settings() { }
	virtual NodeInfo getCurrentNodeInfo() = 0;
	virtual NodeInfo getNodeInfo(int nodeID) = 0;
	virtual NodeMap getNeighbors() = 0;
	virtual int getNodeId() const = 0;
};

}
}


#endif /* HELPER_SETTINGS_SETTINGS_H_ */
