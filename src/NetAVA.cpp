/**
 * @file 	NetAVA.cpp
 * @author 	Tobias Müller
 * @date 	27.10.2016
 */

#include <unistd.h>
#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

#include "core/NodeCore.h"
#include "helper/neighborFinders/GraphvizNeighborsCreator.h"
#include "helper/settings/NodeElectionSettings.h"
#include "helper/settings/InitiatorSettings.h"
#include "helper/utilities/utilities.h"
#include "initiator/Initiator.h"
#include "listener/NodeListener.h"
#include "network/AsyncronousNodeTransceiver.h"


using namespace std;
using namespace core;
using namespace network;
using namespace helper::neighborFinders;

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
			helper::settings::InitiatorSettings settings(addressFilename);
			cout << "init core configuration..." << endl;
			cout << "init core application..." << endl;


			if (type == "listener") {
				cout << "start Listener..." << endl;
				TransceiverBase* transceiver = new AsyncronousNodeTransceiver("127.0.0.1", 5000, 30);
				helper::listener::NodeListener listener(transceiver);
				listener.loop();
			} else if (type == "initiator") {
				cout << "start Initiator..." << endl;
				Initiator initiator(&settings);
				initiator.loop();
			}
		} catch (std::exception& e) {
			helper::utilities::writeLog("Allgemeiner Fehler: ", e);
		}
	} else {
		try {
			nodeID = stoi(argv[3]);
			string config(argv[4]);

			ISearchNeighbors* neighborSearcher = new helper::neighborFinders::GraphvizNeighborsCreator(argv[5]);
			helper::settings::NodeElectionSettings settings(addressFilename, nodeID, neighborSearcher, config);
			NodeCore node(&settings);
			node.loop();

		} catch (std::exception& e) {
			helper::utilities::writeLog("Allgemeiner Fehler: ", e);
		}

		//cout << nodeID << " - Finish" << endl;
	}

	return 0;
}


