/*
 * ConcurrentQueue.h
 *
 *  Created on: 27.01.2017
 *      Author: tobias
 */

#ifndef HELPER_UTILITIES_CONCURRENTQUEUE_H_
#define HELPER_UTILITIES_CONCURRENTQUEUE_H_


#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace helper { namespace utilities {

template<typename T>
class ConcurrentQueue {
public:

	T pop() {
		std::unique_lock<std::mutex> mlock(mutex_);
		while (queue_.empty()) {
			cond_.wait(mlock);
		}
		auto item = queue_.front();
		queue_.pop();
		return item;
	}

	void pop(T& item) {
		std::unique_lock<std::mutex> mlock(mutex_);
		while (queue_.empty()) {
			cond_.wait(mlock);
		}
		item = queue_.front();
		queue_.pop();
	}

	void push(const T& item) {
		std::unique_lock<std::mutex> mlock(mutex_);
		queue_.push(item);
		mlock.unlock();
		cond_.notify_one();
	}

	void push(T&& item) {
		std::unique_lock<std::mutex> mlock(mutex_);
		queue_.push(std::move(item));
		mlock.unlock();
		cond_.notify_one();
	}

private:
	std::queue<T> queue_;
	std::mutex mutex_;
	std::condition_variable cond_;
};

}
}



#endif /* HELPER_UTILITIES_CONCURRENTQUEUE_H_ */
