/*
 * NodeConfigReader.cpp
 *
 *  Created on: 27.10.2016
 *      Author: tobias
 */

#include "NodeBaseSettings.h"

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cctype>
#include <fstream>
#include <map>
#include <utility>
#include <vector>

#include "../../network/NodeTransceiver.h"
#include "../exception/ConfigurationException.h"
#include "../neighborFinders/ISearchNeighbors.h"
#include "../string/trim.h"

namespace helper {
namespace settings {

NodeBaseSettings::NodeBaseSettings(const string& filename)
	: NodeBaseSettings(filename, -1, nullptr, "") {
}

NodeBaseSettings::NodeBaseSettings(const string& filename, int nodeID, ISearchNeighbors* neighborSearcher, string config) {
	this->nodeID = nodeID;

	if (config.size() > 0) {
		this->config = stoi(config);
	}

	this->allNodes = readFile(filename);
	this->neighborSearcher = neighborSearcher;

	if (neighborSearcher != nullptr) {
		this->neighborSearcher->build(nodeID, allNodes);
	}
}

NodeBaseSettings::~NodeBaseSettings() {
	if (neighborSearcher != nullptr)
		delete neighborSearcher;
}

NodeInfo NodeBaseSettings::getCurrentNodeInfo() {
	return getNodeInfo(this->nodeID);
}

NodeMap NodeBaseSettings::getNeighbors() {
	if (neighborSearcher != nullptr)
		return neighborSearcher->getNeighbors(this->nodeID);
	else
		return this->allNodes;
}

NodeInfo NodeBaseSettings::getNodeInfo(int nodeID) {
	return (NodeInfo)this->allNodes[nodeID];
}

NodeMap NodeBaseSettings::readFile(const string& filename) {
	NodeMap nodes;

	string line;
	ifstream file(filename);
	if (file.is_open()) {
		while (getline(file, line)) {
			if (!line.empty()) {
				NodeInfo nodeInfo = readLine(line);
				nodes.insert(NodePair(nodeInfo.NodeID, nodeInfo));
			}
		}
		file.close();
	}

	return nodes;
}

//TODO: Handling der Adresse herausziehen
NodeInfo NodeBaseSettings::readLine(const string& line) {
	NodeInfo nodeInfo;

	vector<string> splittedLine = split(line, " ");

	if (splittedLine.size() != 2)
		throw exception::ConfigurationException("Bad file format!");

	nodeInfo.NodeID = stoi(splittedLine.at(0));

	splittedLine = split(splittedLine.at(1), ":");
	if (splittedLine.size() != 2)
			throw exception::ConfigurationException("Bad file format!");

	string ipStr = splittedLine.at(0);
	if (isdigit(ipStr.at(0))) {
		inet_pton(AF_INET, ipStr.c_str(), &(nodeInfo.Address.sin_addr));
	} else {
		struct addrinfo *result;
		getaddrinfo(ipStr.c_str(), NULL, NULL, &result);
		nodeInfo.Address = *(struct sockaddr_in*)result->ai_addr;
		freeaddrinfo(result);
	}

	nodeInfo.Address.sin_port = stoi(splittedLine.at(1));
	nodeInfo.Address.sin_family = AF_INET;

	return nodeInfo;
}

network::TransceiverBase* NodeBaseSettings::getTransceiver() {
	bool isReceiver = this->nodeID < 0 ? false : true;
	return new network::NodeTransceiver(this->getCurrentNodeInfo(), numberOfConnections, this->allNodes, isReceiver);
}

}
} /* namespace helper */


