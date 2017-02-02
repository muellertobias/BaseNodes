/*
 * MessagePriorityConcurrentQueue.h
 *
 *  Created on: 01.02.2017
 *      Author: tobias
 */

#ifndef MESSAGE_MESSAGEPRIORITYCONCURRENTQUEUE_H_
#define MESSAGE_MESSAGEPRIORITYCONCURRENTQUEUE_H_

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "Message.h"
#include "ControlMessage.h"
#include "ApplicationMessage.h"
#include <iostream>

namespace message {

struct MessagePriorisation
{
	bool operator()(Message* left, Message* right) {
		return dynamic_cast<ApplicationMessage*>(left) != NULL && dynamic_cast<ControlMessage*>(right) != NULL;
	}
};

class MessagePriorityConcurrentQueue {
public:
	MessagePriorityConcurrentQueue() { }
	virtual ~MessagePriorityConcurrentQueue() { }

	Message* pop() {
		std::unique_lock<std::mutex> mlock(mutex_);
		while (queue_.empty()) {
			cond_.wait(mlock);
		}
		auto item = queue_.top();
		queue_.pop();
		return item;
	}

	void pop(Message* item) {
		std::unique_lock<std::mutex> mlock(mutex_);
		while (queue_.empty()) {
			cond_.wait(mlock);
		}
		item = queue_.top();
		queue_.pop();
	}

	void push(Message* item) {
		std::unique_lock<std::mutex> mlock(mutex_);
		queue_.push(item);
		mlock.unlock();
		cond_.notify_one();
	}

	size_t length() const {
		return queue_.size();
	}

	void clear() {
		std::priority_queue<Message*, std::vector<Message*>, MessagePriorisation> empty;
		std::swap(queue_, empty );
	}

private:
	std::priority_queue<Message*, std::vector<Message*>, MessagePriorisation> queue_;
	std::mutex mutex_;
	std::condition_variable cond_;
};

} /* namespace message */

#endif /* MESSAGE_MESSAGEPRIORITYCONCURRENTQUEUE_H_ */
