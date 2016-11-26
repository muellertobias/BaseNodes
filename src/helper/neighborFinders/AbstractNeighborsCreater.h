/*
 * AbstractNeighborsCreater.h
 *
 *  Created on: 16.11.2016
 *      Author: tobias
 */

#ifndef HELPER_NEIGHBORFINDERS_ABSTRACTNEIGHBORSCREATER_H_
#define HELPER_NEIGHBORFINDERS_ABSTRACTNEIGHBORSCREATER_H_

#include "ISearchNeighbors.h"
#include "../../NodeInfo.h"

namespace helper {
namespace neighborFinders {

class AbstractNeighborsCreater : public ISearchNeighbors {
public:
	virtual ~AbstractNeighborsCreater() { }
protected:
	NodeMap neighbors;
};

}
}


#endif /* HELPER_NEIGHBORFINDERS_ABSTRACTNEIGHBORSCREATER_H_ */
