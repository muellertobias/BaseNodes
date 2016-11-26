/*
 * RandomNeighborsCreator.h
 *
 *  Created on: 30.10.2016
 *      Author: tobias
 */

#ifndef HELPER_NEIGHBORFINDERS_RANDOMNEIGHBORSCREATOR_H_
#define HELPER_NEIGHBORFINDERS_RANDOMNEIGHBORSCREATOR_H_

#include "../../NodeInfo.h"
#include "AbstractNeighborsCreater.h"


namespace helper {
namespace neighborFinders {

class RandomNeighborsCreator: public AbstractNeighborsCreater {
public:
	RandomNeighborsCreator(int requestedNumberOfNeighbors);
	virtual ~RandomNeighborsCreator();

	virtual void build(int nodeID, const NodeMap& nodes);
	virtual const NodeMap& getNeighbors(int nodeID);
private:
	int requestedNumberOfNeighbors;
};

}
} /* namespace helper */

#endif /* HELPER_NEIGHBORFINDERS_RANDOMNEIGHBORSCREATOR_H_ */
