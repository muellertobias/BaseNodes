/*
 * NodeInfo.h
 *
 *  Created on: 27.10.2016
 *      Author: tobias
 */

#ifndef NODEINFO_H_
#define NODEINFO_H_

#include <map>
#include <netinet/in.h>

struct NodeInfo {
	int NodeID;
	sockaddr_in Address;
};

typedef std::pair<int, NodeInfo> NodePair;
typedef std::map<NodePair::first_type, NodePair::second_type> NodeMap;



#endif /* NODEINFO_H_ */
