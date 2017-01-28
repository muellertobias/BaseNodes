/*
 * AsyncronousNodeTransceiver.h
 *
 *  Created on: 27.01.2017
 *      Author: tobias
 */

#ifndef NETWORK_ASYNCRONOUSNODETRANSCEIVER_H_
#define NETWORK_ASYNCRONOUSNODETRANSCEIVER_H_

#include <string>
#include <thread>
#include <vector>
#include <algorithm>

#include "TransceiverBase.h"
#include "../helper/utilities/ConcurrentQueue.h"

namespace network {

using namespace helper::utilities;

class AsyncronousNodeTransceiver: public TransceiverBase {
public:
	AsyncronousNodeTransceiver(TransceiverBase* impl);
	virtual ~AsyncronousNodeTransceiver();

	virtual std::string receive();
	virtual bool sendTo(const NodeInfo& destination, const std::string& message);
	virtual bool closeReceiver();
	virtual void resolve(const NodeInfo& nodeInfo, std::string& address);
	virtual void resolve(const int& nodeID, NodeInfo& nodeInfo);

private:
	TransceiverBase* impl;
	ConcurrentQueue<std::string> queue;
	bool isRunning;
	std::thread receiverThread;
	std::vector<std::thread> workers;

	void asyncReceive();

};

} /* namespace network */

#endif /* NETWORK_ASYNCRONOUSNODETRANSCEIVER_H_ */
