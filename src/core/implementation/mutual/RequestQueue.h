/*
 * RequestQueue.h
 *
 *  Created on: 14.02.2017
 *      Author: tobias
 */

#ifndef CORE_IMPLEMENTATION_MUTUAL_REQUESTQUEUE_H_
#define CORE_IMPLEMENTATION_MUTUAL_REQUESTQUEUE_H_


#include <queue>
#include <iostream>
#include <map>

namespace core {
namespace implementation {
namespace mutual {


typedef struct {
	int NodeID;
	int Timestamp;
} Request;

struct RequestPriorisation
{
	bool operator()(Request* left, Request* right) {
		if (left->Timestamp == right->Timestamp) {
			return left->NodeID < right->NodeID;
		}

		return left->Timestamp < right->Timestamp;
	}
};

class RequestQueue {
public:
	RequestQueue(int highestNodeID) : highestNodeID(highestNodeID) {
		clearRequestCounter();
	}
	virtual ~RequestQueue() { }

	Request* pop() {
		auto item = queue_.top();
		queue_.pop();
		requestCounter.at(item->NodeID)--;
		return item;
	}

	void pop(Request* item) {
		item = queue_.top();
		queue_.pop();
		requestCounter.at(item->NodeID)--;
	}

	void push(Request* item) {
		queue_.push(item);
		requestCounter.at(item->NodeID)++;
	}

	size_t length() const {
		return queue_.size();
	}

	void clear() {
		std::priority_queue<Request*, std::vector<Request*>, RequestPriorisation> empty;
		std::swap(queue_, empty);
	}

	void remove(int NodeID) {
		std::priority_queue<Request*, std::vector<Request*>, RequestPriorisation> copy;
		requestCounter.clear();

		clearRequestCounter();
		while (queue_.size() > 0) {
			Request* r = pop();
			if (r->NodeID != NodeID) {
				copy.push(r);
				requestCounter.at(r->NodeID)++;
			} else {
				delete r;
			}
		}

		std::swap(queue_, copy);
	}

	bool containsRequestFromAll() {
		for (map<int,int>::const_iterator it = requestCounter.begin(); it != requestCounter.end(); ++it) {
			if (it->second == 0) {
				return false;
			}
		}
		return true;
	}

	bool topIsFrom(int NodeID) {
		auto item = queue_.top();
		return item->NodeID == NodeID;
	}

private:
	std::priority_queue<Request*, std::vector<Request*>, RequestPriorisation> queue_;
	map<int, int> requestCounter;
	int highestNodeID;

	void clearRequestCounter() {
		for (int i = 1; i <= highestNodeID; ++i) {
			requestCounter.insert(pair<int,int>(i, 0));
		}
	}
};

} /* namespace mutual */
} /* namespace implementation */
} /* namespace core */
#endif /* CORE_IMPLEMENTATION_MUTUAL_REQUESTQUEUE_H_ */
