/*
 * ElectionConstants.h
 *
 *  Created on: 21.01.2017
 *      Author: tobias
 */

#ifndef CORE_IMPLEMENTATION_ELECTION_ELECTIONTYPES_H_
#define CORE_IMPLEMENTATION_ELECTION_ELECTIONTYPES_H_

#include <map>

namespace core {
namespace implementation {
namespace election {

typedef std::map<int, int> ConfidenceLevels;

typedef struct {
	const int party1;
	int confidenceLevel1;
	const int party2;
	int confidenceLevel2;
} Politics;

}
}
}


#endif /* CORE_IMPLEMENTATION_ELECTION_ELECTIONTYPES_H_ */
