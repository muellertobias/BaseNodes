/*
 * NodeTransceiver.cpp
 *
 *  Created on: 02.11.2016
 *      Author: tobias
 */

#include "NodeTransceiver.h"
#include "Message.h"
#include <vector>

namespace network {

NodeTransceiver::NodeTransceiver(const NodeInfo& nodeInfo, const int& numberOfConnections) {
	createReceiver(nodeInfo, numberOfConnections);
}

NodeTransceiver::~NodeTransceiver() {

}

string NodeTransceiver::receive() const {
	struct sockaddr_in client;

	socklen_t length = sizeof(client);

	int clientSocketID = accept(socketID, (struct sockaddr*)&client, &length);

	vector<char> buffer(1024);
	int receivedSize;

	receivedSize = recv(clientSocketID, buffer.data(), buffer.size(), 0);
	close(clientSocketID);

	string msg(buffer.begin(), buffer.end());

	return msg;
}

bool NodeTransceiver::sendTo(const NodeInfo& destination, const string& message) {
	int socketID = socket(AF_INET, SOCK_STREAM, 0);
	if (socketID < 0) {
		cerr << "Error: socket" << endl;
		return false;
	}

	if (connect(socketID, (struct sockaddr*)&destination.Address, sizeof(destination.Address)) < 0) {
		cerr << "Error: connect" << endl;
		return false;
	}

	vector<char> cstr(message.c_str(), message.c_str() + message.size() + 1);

	if (send(socketID, cstr.data(), cstr.size(), 0) < 0) {
		cerr << "Error: send" << endl;
		close(socketID);
		return false;
	}
	close(socketID);
	return true;
}


bool NodeTransceiver::createReceiver(const NodeInfo& nodeInfo,
		const int& numberOfConnections) {

	socketID = socket(AF_INET, SOCK_STREAM, 0);
	if (socketID < 0) {
		return false;
	}

	bind(socketID, (struct sockaddr*)&nodeInfo.Address, sizeof(nodeInfo.Address));
	listen(socketID, numberOfConnections);

	return true;
}

bool NodeTransceiver::closeReceiver() {
	close(this->socketID);
	return true;
}

} /* namespace network */


