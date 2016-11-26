/*
 * IBuildGraph.h
 *
 *  Created on: 30.10.2016
 *      Author: tobias
 */

#ifndef HELPER_NEIGHBORFINDERS_ISEARCHNEIGHBORS_H_
#define HELPER_NEIGHBORFINDERS_ISEARCHNEIGHBORS_H_

#include "../../NodeInfo.h"

namespace helper {
namespace neighborFinders {


class ISearchNeighbors {
public:
	virtual ~ISearchNeighbors() { }
	virtual void build(int nodeID, const NodeMap& nodes) = 0;
	virtual const NodeMap& getNeighbors(int nodeID) = 0;
};

}
}

#endif /* HELPER_NEIGHBORFINDERS_ISEARCHNEIGHBORS_H_ */
