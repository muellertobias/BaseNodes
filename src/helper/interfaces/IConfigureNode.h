/*
 * IConfigureNode.h
 *
 *  Created on: 30.10.2016
 *      Author: tobias
 */

#ifndef HELPER_INTERFACES_ICONFIGURENODE_H_
#define HELPER_INTERFACES_ICONFIGURENODE_H_


namespace helper {
namespace interfaces {

class IConfigureNode {
public:
	virtual ~IConfigureNode() { }
	virtual NodeInfo getCurrentNodeInfo() = 0;
	virtual NodeInfo getNodeInfo(int nodeID) = 0;
	virtual NodeMap getNeighbors() = 0;
	virtual int getNodeId() const = 0;
};

}
}


#endif /* HELPER_INTERFACES_ICONFIGURENODE_H_ */
