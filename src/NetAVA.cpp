/**
 * @file 	NetAVA.cpp
 * @author 	Tobias Müller
 * @date 	27.10.2016
 */

#include <iostream>
#include <string>
#include <vector>

#include "core/implementation/NodeCoreBaseImpl.h"
#include "core/implementation/rumor/RumorNodeCoreImpl.h"
#include "core/NodeCore.h"
#include "helper/exception/NodeBaseException.h"
#include "helper/neighborFinders/GraphvizNeighborsCreator.h"
#include "initiator/Initiator.h"
#include "listener/NodeListener.h"
#include "network/NodeTransceiver.h"
#include "helper/Constants.h"
#include "helper/settings/NodeBaseSettings.h"


using namespace std;
using namespace core;
using namespace network;
using namespace helper::exception;
using namespace helper::neighborFinders;

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
		cout << "NetAVA address_file type node_id impl conf graph_file" << endl;

		cout << "Types: node listener initiator" << endl;
		cout << "Implementations: candidate voter" << endl;
		cout << "Config: candidate r={1..n}, voter {ignored}" << endl;

		return -1;
	}

	string addressFilename(argv[1]);
	string type(argv[2]);

	if (argc == 3) {
		try {
			cout << "load configure..." << endl;
			helper::settings::NodeBaseSettings configReader(addressFilename);
			cout << "init core configuration..." << endl;
			NodeCore core(&configReader);
			cout << "init core application..." << endl;


			if (type == "listener") {
				cout << "start Listener..." << endl;
				TransceiverBase* transceiver = new NodeTransceiver("127.0.0.1", 5000, 10);
				helper::listener::NodeListener listener(transceiver);
				listener.loop();
			} else if (type == "initiator") {
				cout << "start Initiator..." << endl;
				Initiator initiator(core);
				initiator.loop();
			}
		} catch (std::exception& e) {
			cerr << "Fehler: " << e.what() << endl;
		}
	} else {
		try {
			nodeID = stoi(argv[3]);
			string impl(argv[4]);

			ISearchNeighbors* neighborSearcher = new helper::neighborFinders::GraphvizNeighborsCreator(argv[6]);

			helper::settings::NodeBaseSettings configReader(addressFilename, nodeID, neighborSearcher);

			// Implementierung anlegen
			implementation::INodeImpl* nodeImpl;


			if (impl == "rumor") {
				int threshold = stoi(argv[5]);
				nodeImpl = new core::implementation::rumor::RumorNodeCoreImpl(threshold);
			} else {
				nodeImpl = new core::implementation::NodeCoreBaseImpl();
			}

			NodeCore node(&configReader, nodeImpl);
			node.loop();

		} catch (NodeBaseException& e) {
			cerr << e.what() << endl;
		} catch (std::exception& e) {
			cerr << "Allgemeiner Fehler: " << e.what() << endl;
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

