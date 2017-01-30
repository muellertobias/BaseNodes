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
#include <map>
#include <algorithm>

#include "TransceiverBase.h"
#include "../helper/utilities/ConcurrentQueue.h"

namespace network {

using namespace std;
using namespace helper::utilities;

typedef map<int, int> Transporter; // NodeID & SocketID
typedef map<int, thread> ReceiverThreads;

class AsyncronousNodeTransceiver: public TransceiverBase {
public:
	AsyncronousNodeTransceiver(const std::string& address, const int& port, const int& numberOfConnections, bool isReceiver = true);
	AsyncronousNodeTransceiver(const NodeInfo& nodeInfo, const int& numberOfConnections, const NodeMap& staticNames, bool isReceiver = true);
	virtual ~AsyncronousNodeTransceiver();

	virtual std::string receive();
	virtual bool sendTo(const NodeInfo& destination, const string& message);
	virtual bool closeReceiver();
	virtual void resolve(const NodeInfo& nodeInfo, string& address);
	virtual void resolve(const int& nodeID, NodeInfo& nodeInfo);

private:
	ConcurrentQueue<string> receiverQueue;

	int socketID;
	bool isRunning; //TODO Mutual machen
	NodeMap staticNameService;
	thread receiverThread;
	//vector<thread> receivers;
	ReceiverThreads receivers;
	Transporter senders;

	void asyncReceive();
	int createConnection(const NodeInfo& destination);
	void createReceiver(const NodeInfo& nodeInfo, const int& numberOfConnections);
	void createReceiver(const std::string& address, const int& port, const int& numberOfConnections);

	bool sendTo(int socketID, const string& message);
	void receive(int clientSocketID);
};

} /* namespace network */

#endif /* NETWORK_ASYNCRONOUSNODETRANSCEIVER_H_ */
