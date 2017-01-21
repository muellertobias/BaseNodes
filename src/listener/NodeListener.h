/*
 * NodeListener.h
 *
 *  Created on: 11.01.2017
 *      Author: tobias
 */

#ifndef LISTENER_NODELISTENER_H_
#define LISTENER_NODELISTENER_H_

#include "../message/Message.h"
#include "../network/TransceiverBase.h"
#include "../network/NodeTransceiver.h"

namespace helper {
namespace listener {

using namespace network;

class NodeListener {
public:
	NodeListener(TransceiverBase* transceiver);
	virtual ~NodeListener();

	void loop();

private:
	TransceiverBase* transceiver;

	Message receive() const;
	void print(const Message& message);
};

} /* namespace listener */
} /* namespace helper */

#endif /* LISTENER_NODELISTENER_H_ */
