/*
 * NodeListener.h
 *
 *  Created on: 11.01.2017
 *      Author: tobias
 */

#ifndef LISTENER_NODELISTENER_H_
#define LISTENER_NODELISTENER_H_

#include "../network/TransceiverBase.h"
#include "../message/Message.h"
#include "../message/ControlMessage.h"
#include "../message/ApplicationMessage.h"
#include "../helper/string/trim.h"

namespace helper {
namespace listener {

using namespace message;
using namespace network;


class NodeListener {
public:
	NodeListener(TransceiverBase* transceiver);
	virtual ~NodeListener();

	void loop();

private:
	TransceiverBase* transceiver;
	bool isRunning;

	void handle(ControlMessage* message);
	void print(Message* const message);
};

} /* namespace listener */
} /* namespace helper */

#endif /* LISTENER_NODELISTENER_H_ */
