/*
 * VectorTime.cpp
 *
 *  Created on: 20.01.2017
 *      Author: tobias
 */

#include "VectorTime.h"

#include <algorithm>

namespace helper {
namespace time {

VectorTime::VectorTime() : VectorTime(0) {
}

VectorTime::VectorTime(const int& localID) : localID(localID) {
	setTime(localID, 0);
}

VectorTime::~VectorTime() {
	// TODO Auto-generated destructor stub
}

void VectorTime::setTime(const int& id, const int& time) {
	if (id <= 0) {
		return;
	}

	VectorTimeMap::iterator it = timeMap.find(id);

	if (it != timeMap.end()) {
		if (it->second < time) {
			it->second = time;
		}
	} else {
		this->timeMap.insert(VectorTimePair(id, time));
	}
}

int VectorTime::getTime(const int& id) {
	VectorTimeMap::iterator it = timeMap.find(id);

	if (it != timeMap.end()) {
		return it->second;
	}
	return -1;
}

int VectorTime::getLocalTime() {
	if (localID <= 0)
		return -1;

	return timeMap.at(localID);
}

void VectorTime::increase(int id) {
	VectorTimeMap::iterator it = timeMap.find(id);
	if (it != timeMap.end()) {
		it->second++;
	} else {
		timeMap.insert(VectorTimePair(id, 1));
	}
}

void VectorTime::increase() {
	increase(localID);
}

void VectorTime::merge(const VectorTimeMap& otherTimeMap) {
	for (VectorTimeMap::const_iterator it = otherTimeMap.begin(); it != otherTimeMap.end(); it++) {
		setTime(it->first, it->second);
	}
}

int VectorTime::getMaximum() {
	VectorTimeMap::iterator max = std::max_element(timeMap.begin(), timeMap.end(), [](const VectorTimePair& p1, const VectorTimePair& p2) {
        return p1.second < p2.second;
    });
	return max->second;
}

} /* namespace time */
} /* namespace helper */

