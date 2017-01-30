/*
 * AsyncronousNodeTransceiver.cpp
 *
 *  Created on: 27.01.2017
 *      Author: tobias
 */

#include "AsyncronousNodeTransceiver.h"

#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <cctype>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <utility>

#include "../helper/exception/NetworkException.h"
#include "../helper/NodeInfo.h"
#include "../helper/utilities/utilities.h"

namespace network {

AsyncronousNodeTransceiver::AsyncronousNodeTransceiver(const std::string& address,
		const int& port, const int& numberOfConnections, bool isReceiver) : isRunning(isReceiver) {
	if (isReceiver) {
		createReceiver(address, port, numberOfConnections);
		this->receiverThread = thread(&AsyncronousNodeTransceiver::asyncReceive, this);
	}
}

AsyncronousNodeTransceiver::AsyncronousNodeTransceiver(const NodeInfo& nodeInfo, const int& numberOfConnections,
		const NodeMap& staticNames, bool isReceiver)
	: isRunning(isReceiver), staticNameService(staticNames) {
	if (isReceiver) {
		createReceiver(nodeInfo, numberOfConnections);
		this->receiverThread = thread(&AsyncronousNodeTransceiver::asyncReceive, this);
	}

}

AsyncronousNodeTransceiver::~AsyncronousNodeTransceiver() {
	if (isRunning) {
		closeReceiver();
	}
}

std::string AsyncronousNodeTransceiver::receive() {
	return this->receiverQueue.pop();
}

bool AsyncronousNodeTransceiver::sendTo(const NodeInfo& destination,
		const string& message) {

	Transporter::iterator sender = senders.find(destination.NodeID);
	int socketID = 0;
	if (sender != senders.end()) {
		socketID = sender->second;
	} else {
		socketID = createConnection(destination);
		senders.insert(pair<int, int>(destination.NodeID, socketID));
	}

	if (!sendTo(socketID, message)) {
		helper::utilities::writeLog(__FUNCTION__, "Error: send to " + to_string(destination.NodeID));
		senders.erase(destination.NodeID);
		return false;
	}
	return true;
}

bool AsyncronousNodeTransceiver::closeReceiver() {
	try {
		isRunning = false;
		std::for_each(senders.begin(), senders.end(), [](std::pair<const int, int>& s) {
			close(s.second);
		});
		shutdown(socketID, SHUT_RDWR);
		close(socketID);
		for (ReceiverThreads::iterator it = receivers.begin(); it != receivers.end(); it++) {
			shutdown(it->first, SHUT_RDWR);

			if (it->second.joinable()) {
				it->second.join();
			}
		}
		if (this->receiverThread.joinable()) {
			this->receiverThread.join();
		}

	} catch (std::exception& e) {
		cout << e.what() << endl;
	}


	return true;
}

void AsyncronousNodeTransceiver::resolve(const NodeInfo& nodeInfo, std::string& address) {
	char cstr[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(nodeInfo.Address.sin_addr), cstr, INET_ADDRSTRLEN);

	address.append(cstr);
	address.append(":");
	address.append(to_string(nodeInfo.Address.sin_port));
}

void AsyncronousNodeTransceiver::resolve(const int& nodeID, NodeInfo& nodeInfo) {
	if (nodeID == 0) {
		nodeInfo = this->staticNameService.rbegin()->second;
	} else {
		nodeInfo = this->staticNameService.at(nodeID);
	}
}

void AsyncronousNodeTransceiver::asyncReceive() {
	while (isRunning) {
		try {
			struct sockaddr_in client;

			socklen_t length = sizeof(client);
			int clientSocketID = accept(socketID, (struct sockaddr*)&client, &length);
			if (clientSocketID > 0) {
				receivers.insert(std::pair<int, thread>(clientSocketID, std::thread([clientSocketID, this]() {
					this->receive(clientSocketID);
				})));
			}
		} catch (std::exception& e) {
			cerr << e.what() << endl;
		}
	}
}

int AsyncronousNodeTransceiver::createConnection(const NodeInfo& destination) {
	int socketID = socket(AF_INET, SOCK_STREAM, 0);
	if (socketID < 0) {
		helper::utilities::writeLog(__FUNCTION__, "Error: socket to " + to_string(destination.NodeID));
		return -1;
	}

	if (connect(socketID, (struct sockaddr*)&destination.Address, sizeof(destination.Address)) < 0) {
		helper::utilities::writeLog(__FUNCTION__, "Error: connect to " + to_string(destination.NodeID) + " - " + strerror(errno));
		return -1;
	}
	return socketID;
}

bool AsyncronousNodeTransceiver::sendTo(int socketID, const string& message) {

	std::vector<char> cstr(message.c_str(), message.c_str() + message.size() + 1);

	// Retry einbauen
	if (send(socketID, cstr.data(), cstr.size(), 0) < 0) {
		close(socketID);
		return false;
	}
	return true;
}

void AsyncronousNodeTransceiver::receive(int clientSocketID) {
	while (this->isRunning) {
		string message;
		vector<char> buffer(1024);
		int receivedSize;

		do {
			memset(buffer.data(), '\0', buffer.size());
			receivedSize = recv(clientSocketID, buffer.data(), buffer.size(), 0);

			if (errno) {
				cout << strerror(errno) << endl;
				string errnoStr(strerror(errno));
				throw helper::exception::NetworkException("Receive failed with: " + errnoStr);
			}

			if (receivedSize > 0)
				message.append(buffer.begin(), buffer.end());
		} while (receivedSize == buffer.size());

		if (message.length() > 0)
			this->receiverQueue.push(message);
	}
}

void AsyncronousNodeTransceiver::createReceiver(const NodeInfo& nodeInfo, const int& numberOfConnections) {

	socketID = socket(AF_INET, SOCK_STREAM, 0);
	if (socketID < 0) {
		cout << strerror(errno) << endl;
		throw helper::exception::NetworkException("Creating socket failed!");
	}

	int yes = 1;
    setsockopt(socketID, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

	int isBound = bind(socketID, (struct sockaddr*)&nodeInfo.Address, sizeof(nodeInfo.Address));
	if (isBound < 0) {
		cout << strerror(errno) << endl;
		throw helper::exception::NetworkException("Blocked Socket! - Bind");
	}
	int isListening = listen(socketID, numberOfConnections);
	if (isListening < 0) {
		throw helper::exception::NetworkException("Blocked Socket! - Listen");
	}

	fd_set rfds;
	struct timeval tv;
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	FD_ZERO(&rfds);
	FD_SET(socketID, &rfds);
	select(socketID + 1, &rfds, NULL, NULL, &tv);
}

void AsyncronousNodeTransceiver::createReceiver(const std::string& address,
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


} /* namespace network */

