/*
 * VectorTime.h
 *
 *  Created on: 20.01.2017
 *      Author: tobias
 */

#ifndef NETWORK_VECTORTIME_H_
#define NETWORK_VECTORTIME_H_

#include <map>

namespace helper {
namespace time {

typedef std::pair<int, int> VectorTimePair;
typedef std::map<VectorTimePair::first_type, VectorTimePair::second_type> VectorTimeMap;

class VectorTime {
public:
	VectorTime();
	VectorTime(const int& localID);
	virtual ~VectorTime();

	void setTime(const int& nodeID, const int& time);
	int getTime(const int& nodeID);
	int getLocalTime();


	void increase(int id);
	void increase();

	const VectorTimeMap& getTimeMap() const {
		return timeMap;
	}

	void overrideTimeMap(const VectorTimeMap& otherTimeMap) {
		this->timeMap = timeMap;
	}


	void merge(const VectorTimeMap& timeMap);

private:
	const int& localID;
	VectorTimeMap timeMap;
};

} /* namespace time */
} /* namespace helper */

#endif /* NETWORK_VECTORTIME_H_ */
