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

enum Party {
	Red,
	Blue
};

typedef std::map<Party, int> ConfidenceLevels;

}
}
}


#endif /* CORE_IMPLEMENTATION_ELECTION_ELECTIONTYPES_H_ */
