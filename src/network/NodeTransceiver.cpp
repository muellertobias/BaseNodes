/*
 * NodeTransceiver.cpp
 *
 *  Created on: 02.11.2016
 *      Author: tobias
 */

#include "NodeTransceiver.h"
#include "Message.h"
#include <vector>
#include "../helper/exception/NetworkException.h"

namespace network {

NodeTransceiver::NodeTransceiver(const std::string& address, const int& port,
		const int& numberOfConnections, bool isReceiver) {
	if (isReceiver) {
		createReceiver(address, port, numberOfConnections);
	}
}

NodeTransceiver::NodeTransceiver(const NodeInfo& nodeInfo, const int& numberOfConnections, bool isReceiver) {
	if (isReceiver) {
		createReceiver(nodeInfo, numberOfConnections);
	}
}

NodeTransceiver::~NodeTransceiver() {
	closeReceiver();
}

string NodeTransceiver::receive() const {
	string msg;

	struct sockaddr_in client;

	socklen_t length = sizeof(client);

	int clientSocketID = accept(socketID, (struct sockaddr*)&client, &length);

	vector<char> buffer(1024);
	size_t receivedSize;

	do {
		memset(buffer.data(), '\0', buffer.size());
		receivedSize = recv(clientSocketID, buffer.data(), buffer.size(), 0);
		msg.append(buffer.begin(), buffer.end());
	} while (receivedSize == buffer.size());

	close(clientSocketID);
	cout << "Received: "<< msg << endl;
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

string NodeTransceiver::resolve(const NodeInfo& nodeInfo) const {
	char cstr[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(nodeInfo.Address.sin_addr), cstr, INET_ADDRSTRLEN);

	string addressStr(cstr);
	addressStr.append(":");
	addressStr.append(to_string(nodeInfo.Address.sin_port));
	return addressStr;
}

bool NodeTransceiver::createReceiver(const std::string& address,
		const int& port, const int& numberOfConnections) {
	NodeInfo nodeInfo;
	nodeInfo.NodeID = 0;

	if (isdigit(address.at(0))) {
		inet_pton(AF_INET, address.c_str(), &(nodeInfo.Address.sin_addr));
	} else {
		struct addrinfo *result;
		getaddrinfo(address.c_str(), NULL, NULL, &result);
		nodeInfo.Address = *(struct sockaddr_in*)result->ai_addr;
		freeaddrinfo(result);
	}

	nodeInfo.Address.sin_port = port;
	nodeInfo.Address.sin_family = AF_INET;

	return createReceiver(nodeInfo, numberOfConnections);
}

bool NodeTransceiver::createReceiver(const NodeInfo& nodeInfo,
		const int& numberOfConnections) {

	socketID = socket(AF_INET, SOCK_STREAM, 0);
	if (socketID < 0) {
		return false;
	}

	int b = bind(socketID, (struct sockaddr*)&nodeInfo.Address, sizeof(nodeInfo.Address));
	if (b < 0) {
		cout << strerror(errno) << endl;
		throw helper::exception::NetworkException("Blocked Socket! - Bind");
	}
	int l = listen(socketID, numberOfConnections);
	if (l < 0) {
		throw helper::exception::NetworkException("Blocked Socket! - Listen");
	}

	return true;
}

bool NodeTransceiver::closeReceiver() {
	shutdown(this->socketID, SHUT_RDWR);
	close(this->socketID);
	return true;
}

} /* namespace network */


