/*
 * VectorTime.h
 *
 *  Created on: 20.01.2017
 *      Author: tobias
 */

#ifndef HELPER_TIME_VECTORTIME_H_
#define HELPER_TIME_VECTORTIME_H_

#include <map>

namespace helper {
namespace time {

typedef std::pair<int, int> VectorTimePair;
typedef std::map<VectorTimePair::first_type, VectorTimePair::second_type> VectorTimeMap;

class VectorTime {
public:
	VectorTime();
	VectorTime(const int& localID);
	VectorTime(const VectorTime& time);
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
	int getMaximum();
	static int getMaximum(const VectorTimeMap& timeMap);

	bool isTerminated();

	int getTermininationTime() const {
		return termininationTime;
	}

	void setTermininationTime(int termininationTime) {
		this->termininationTime = termininationTime;
	}

private:
	const int& localID;
	int termininationTime;
	VectorTimeMap timeMap;
};

} /* namespace time */
} /* namespace helper */

#endif /* HELPER_TIME_VECTORTIME_H_ */
