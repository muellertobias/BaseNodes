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
#include "../message/Message.h"

namespace network {

class TransceiverBase {
public:
	virtual ~TransceiverBase() { };
	virtual message::Message* receive() = 0;
	virtual bool sendTo(const NodeInfo& destination, const std::string& message) = 0;
	virtual bool closeReceiver() = 0;
	virtual void resolve(const NodeInfo& nodeInfo, std::string& address) = 0;
	virtual void resolve(const int& nodeID, NodeInfo& nodeInfo) = 0;
};

} /* namespace network */

#endif /* NETWORK_TRANSCEIVERBASE_H_ */
