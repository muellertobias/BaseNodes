/*
 * NodeTransceiver.cpp
 *
 *  Created on: 02.11.2016
 *      Author: tobias
 */

#include "NodeTransceiver.h"
#include "Message.h"
#include <vector>
#include <sstream>
#include "../helper/exception/NetworkException.h"

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
		stringstream sstream;
		sstream << "Send to " << destination.NodeID << ": socket failed";
		throw new helper::exception::NetworkException(sstream.str());
	}

	if (connect(socketID, (struct sockaddr*)&destination.Address, sizeof(destination.Address)) < 0) {
		stringstream sstream;
		sstream << "Send to " << destination.NodeID << ": connect failed";
		throw new helper::exception::NetworkException(sstream.str());
	}

	vector<char> cstr(message.c_str(), message.c_str() + message.size() + 1);

	if (send(socketID, cstr.data(), cstr.size(), 0) < 0) {
		close(socketID);
		stringstream sstream;
		sstream << "Send to " << destination.NodeID << ": send failed";
		throw new helper::exception::NetworkException(sstream.str());
	}

	close(socketID);
	return true;
}

string NodeTransceiver::resolve(const NodeInfo& nodeInfo) const {
	char cstr[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(nodeInfo.Address.sin_addr), cstr, INET_ADDRSTRLEN);

	string addressStr(cstr);
	addressStr.append(":");
	addressStr.append(to_string(nodeInfo.Address.sin_port));
	return addressStr;
}

void NodeTransceiver::createReceiver(const NodeInfo& nodeInfo,
		const int& numberOfConnections) {

	socketID = socket(AF_INET, SOCK_STREAM, 0);
	if (socketID < 0) {
		throw new helper::exception::NetworkException("Create Receiver: socket failed");
	}

	if (bind(socketID, (struct sockaddr*)&nodeInfo.Address, sizeof(nodeInfo.Address)) == -1) {
		throw new helper::exception::NetworkException("Create Receiver: bind failed");
	}

	if (listen(socketID, numberOfConnections) == -1) {
		throw new helper::exception::NetworkException("Create Receiver: listen failed");
	}
}

bool NodeTransceiver::closeReceiver() {
	close(this->socketID);
	return true;
}

} /* namespace network */


