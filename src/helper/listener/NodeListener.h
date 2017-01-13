/*
 * NodeListener.h
 *
 *  Created on: 11.01.2017
 *      Author: tobias
 */

#ifndef HELPER_LISTENER_NODELISTENER_H_
#define HELPER_LISTENER_NODELISTENER_H_

#include "../../network/TransceiverBase.h"
#include "../../network/NodeTransceiver.h"
#include "../../network/Message.h"

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

#endif /* HELPER_LISTENER_NODELISTENER_H_ */
