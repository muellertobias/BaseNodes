/**
 * @file 	NetAVA.cpp
 * @author 	Tobias Müller
 * @date 	27.10.2016
 */
#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include "core/NodeCore.h"
#include "helper/neighborFinders/ExplicitNeighborsCreater.h"
#include "helper/neighborFinders/GraphvizNeighborsCreator.h"
#include "helper/neighborFinders/RandomNeighborsCreator.h"
#include "helper/NodeConfigReader.h"
#include "helper/utilities/utilities.h"
#include "Initiator.h"

using namespace std;
using namespace core;
using namespace network;

/**
 * Main-Methode
 *
 * @param argc Anzahl Übergabeparameter
 * @param argv Übergabeparameter als C-Strings
 * @return 0
 */
int main(int argc, char** argv) {

	int nodeID = 0;

	if (argc == 1) {
		cout << "Wrong Parameters:\n";
		cout << "NetAVA address_file node_id\n";
		cout << "NetAVA address_file node_id neighbor_node_id_1 neighbor_node_id_2 ...\n";
		cout << "NetAVA address_file node_id graph_file" << endl;
		return -1;
	}

	string addressFilename(argv[1]);

	try {
		nodeID = stoi(argv[2]);
		//logging::Logger::getInstance()->setFilename("log-node-" + nodeID + ".txt");

		helper::neighborFinders::ISearchNeighbors* neighborSearcher = nullptr;

		if (argc > 3) {
			if (helper::utilities::isNumber(argv[3])) {
				vector<int> nodeIDs;
				for (int i = 3; i < argc; ++i) {
					int id = stoi(argv[i]);
					nodeIDs.push_back(id);
					cout << "Neighbore NodeID: " << id << endl;
				}
				neighborSearcher = new helper::neighborFinders::ExplicitNeighborsCreater(nodeIDs);
			} else {
				neighborSearcher = new helper::neighborFinders::GraphvizNeighborsCreator(argv[3]);
			}
		} else {
			neighborSearcher = new helper::neighborFinders::RandomNeighborsCreator(3);
		}

		helper::NodeConfigReader configReader(addressFilename, nodeID, neighborSearcher);
		NodeCore node(&configReader);

		if (nodeID > 0) {
			node.loop();
		} else {
			Initiator initiator(node);
			initiator.loop();
		}

	} catch (const exception& e) {
		cerr << "Fehler: " << e.what() << endl;
	}

	cout << "Finish" << endl;
	return 0;
}


