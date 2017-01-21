/*
 * TransceiverBase.h
 *
 *  Created on: 02.11.2016
 *      Author: tobias
 */

#ifndef NETWORK_TRANSCEIVERBASE_H_
#define NETWORK_TRANSCEIVERBASE_H_

#include <string>
#include "../helper/NodeInfo.h"

namespace network {

class TransceiverBase {
public:
	virtual ~TransceiverBase() { };
	virtual std::string receive() const = 0;
	virtual bool sendTo(const NodeInfo& destination, const std::string& message) = 0;
	virtual bool closeReceiver() = 0;
	virtual std::string resolve(const NodeInfo& nodeInfo) const = 0;

protected:
	virtual bool createReceiver(const std::string& address, const int& port, const int& numberOfConnections) = 0;
	virtual bool createReceiver(const NodeInfo& nodeInfo, const int& numberOfConnections) = 0;
};

} /* namespace network */

#endif /* NETWORK_TRANSCEIVERBASE_H_ */
