/**
 * @file 	NodeInfo.h
 * @author 	Tobias Müller
 * @date 	27.10.2016
 */

#ifndef NODEINFO_H_
#define NODEINFO_H_

#include <map>
#include <netinet/in.h>

/**
 * NodeInfo dient zur Adressierung eines Knotens.
 */
struct NodeInfo {
	int NodeID;
	sockaddr_in Address;
};

typedef std::pair<int, NodeInfo> NodePair;
typedef std::map<NodePair::first_type, NodePair::second_type> NodeMap;



#endif /* NODEINFO_H_ */
