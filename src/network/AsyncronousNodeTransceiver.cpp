/*
 * AsyncronousNodeTransceiver.cpp
 *
 *  Created on: 27.01.2017
 *      Author: tobias
 */

#include "AsyncronousNodeTransceiver.h"
#include <iostream>
#include <chrono>

namespace network {

AsyncronousNodeTransceiver::AsyncronousNodeTransceiver(TransceiverBase* impl)
	: impl(impl), isRunning(true) {
	this->receiverThread = std::thread(&AsyncronousNodeTransceiver::asyncReceive, this);

}

AsyncronousNodeTransceiver::~AsyncronousNodeTransceiver() {
	delete impl;
}

std::string AsyncronousNodeTransceiver::receive() {
	return this->queue.pop();
}

bool AsyncronousNodeTransceiver::sendTo(const NodeInfo& destination,
		const std::string& message) {
	//std::this_thread::sleep_for(std::chrono::milliseconds(10));
	workers.push_back(std::thread([destination, message, this]() {
		this->impl->sendTo(destination, message);
	}));
	return true;
}

bool AsyncronousNodeTransceiver::closeReceiver() {
	impl->closeReceiver();
	isRunning = false;
	receiverThread.join();
	receiverThread.detach();
	std::for_each(workers.begin(), workers.end(), [](std::thread &t) {
		t.join();
	});

	return true;
}

void AsyncronousNodeTransceiver::resolve(const NodeInfo& nodeInfo,
		std::string& address) {
	this->impl->resolve(nodeInfo, address);
}

void AsyncronousNodeTransceiver::resolve(const int& nodeID,
		NodeInfo& nodeInfo) {
	this->impl->resolve(nodeID, nodeInfo);
}

void AsyncronousNodeTransceiver::asyncReceive() {
	while (isRunning) {
		try {
			std::string message = this->impl->receive();
			this->queue.push(message);
		} catch (std::exception& e) {

		}
	}
}

} /* namespace network */
