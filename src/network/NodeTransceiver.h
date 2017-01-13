/*
 * NodeTransceiver.h
 *
 *  Created on: 02.11.2016
 *      Author: tobias
 */

#ifndef NETWORK_NODETRANSCEIVER_H_
#define NETWORK_NODETRANSCEIVER_H_

#include "TransceiverBase.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <string>

namespace network {

using namespace std;

class NodeTransceiver: public TransceiverBase {
public:
	NodeTransceiver(const std::string& address, const int& port, const int& numberOfConnections);
	NodeTransceiver(const NodeInfo& nodeInfo, const int& numberOfConnections);
	virtual ~NodeTransceiver();
	virtual string receive() const;
	virtual bool sendTo(const NodeInfo& destination, const string& message);
	virtual bool closeReceiver();
	virtual string resolve(const NodeInfo& nodeInfo) const;

protected:
	virtual bool createReceiver(const std::string& address, const int& port, const int& numberOfConnections);
	virtual bool createReceiver(const NodeInfo& nodeInfo, const int& numberOfConnections);

private:
	int socketID;
};

} /* namespace network */

#endif /* NETWORK_NODETRANSCEIVER_H_ */
