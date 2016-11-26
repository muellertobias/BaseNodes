/*
 * ExplicitNeighborsCreater.h
 *
 *  Created on: 09.11.2016
 *      Author: tobias
 */

#ifndef HELPER_NEIGHBORFINDERS_EXPLICITNEIGHBORSCREATER_H_
#define HELPER_NEIGHBORFINDERS_EXPLICITNEIGHBORSCREATER_H_

#include <string>
#include <vector>

#include "AbstractNeighborsCreater.h"

namespace helper {
namespace neighborFinders {

class ExplicitNeighborsCreater : public AbstractNeighborsCreater {
public:
	ExplicitNeighborsCreater(const std::vector<int>& nodeIDs);
	virtual ~ExplicitNeighborsCreater();

	virtual void build(int nodeID, const NodeMap& nodes);
	virtual const NodeMap& getNeighbors(int nodeID);

private:
	std::vector<int> nodeIDs;
};

}
} /* namespace helper */

#endif /* HELPER_NEIGHBORFINDERS_EXPLICITNEIGHBORSCREATER_H_ */
