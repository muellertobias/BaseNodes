/**
 * @file 	NetAVA.cpp
 * @author 	Tobias Müller
 * @date 	27.10.2016
 */

#include <iostream>
#include <string>
#include <vector>

#include "core/NodeCore.h"
#include "helper/exception/NodeBaseException.h"
#include "helper/neighborFinders/GraphvizNeighborsCreator.h"
#include "helper/settings/NodeElectionSettings.h"
#include "initiator/Initiator.h"
#include "listener/NodeListener.h"
#include "network/NodeTransceiver.h"


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
		cout << "NetAVA address_file type node_id conf graph_file" << endl;

		cout << "Types: node listener initiator" << endl;
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
			string config(argv[4]);

			ISearchNeighbors* neighborSearcher = new helper::neighborFinders::GraphvizNeighborsCreator(argv[5]);
			helper::settings::NodeBaseSettings settings(addressFilename, nodeID, neighborSearcher, config);
			//helper::settings::NodeElectionSettings settings(addressFilename, nodeID, neighborSearcher, config);
			NodeCore node(&settings);
			node.loop();

		} catch (NodeBaseException& e) {
			cerr << e.what() << endl;
		} catch (std::exception& e) {
			cerr << "Allgemeiner Fehler: " << e.what() << endl;
		}
	}

	return 0;
}


