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
#include "helper/listener/NodeListener.h"

using namespace std;
using namespace core;
using namespace network;

void readNeighbors(vector<int>* nodeIDs, int startIndex, int endIndex, char** rawData);

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
		cout << "NetAVA address_file type\n";
		cout
				<< "NetAVA address_file type node_id neighbor_node_id_1 neighbor_node_id_2 ...\n";
		cout << "NetAVA address_file type node_id graph_file" << endl;

		cout << "Types: node listener initiator" << endl;
		return -1;
	}

	string addressFilename(argv[1]);
	string type(argv[2]);

	if (argc == 3) {
		try {
			cout << "load configure..." << endl;
			helper::NodeConfigReader configReader(addressFilename);
			cout << "init core configuration..." << endl;
			NodeCore core(&configReader);
			cout << "init core application..." << endl;


			if (type == "listener") {
				cout << "Starte Listener..." << endl;
				TransceiverBase* transceiver = new NodeTransceiver("127.0.0.1", 4999, 10);
				helper::listener::NodeListener listener(transceiver);
				listener.loop();
			} else if (type == "initiator") {
				cout << "Starte Initiator..." << endl;
				Initiator initiator(core);
				initiator.loop();
			}
		} catch (const exception& e) {
			cerr << "Fehler: " << e.what() << endl;
		}
	} else {
		try {
			nodeID = stoi(argv[3]);

			helper::neighborFinders::ISearchNeighbors* neighborSearcher = nullptr;

			if (argc > 4) {
				if (helper::utilities::isNumber(argv[4])) {
					vector<int> nodeIDs;
					readNeighbors(&nodeIDs, 4, argc, argv);

					neighborSearcher = new helper::neighborFinders::ExplicitNeighborsCreater(nodeIDs);
				} else {
					neighborSearcher = new helper::neighborFinders::GraphvizNeighborsCreator(argv[4]);
				}
			} else {
				neighborSearcher = new helper::neighborFinders::RandomNeighborsCreator(3);
			}

			helper::NodeConfigReader configReader(addressFilename, nodeID, neighborSearcher);
			NodeCore node(&configReader);

			node.loop();
		} catch (const exception& e) {
			cerr << "Fehler: " << e.what() << endl;
		}
	}

	cout << "Finish" << endl;
	return 0;
}

void readNeighbors(vector<int>* nodeIDs, int startIndex, int endIndex, char** rawData) {
	for (int i = startIndex; i < endIndex; ++i) {
		int id = stoi(rawData[i]);
		nodeIDs->push_back(id);
		cout << "Neighbore NodeID: " << id << endl;
	}
}

