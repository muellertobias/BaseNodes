/*
 * InitiatorSettings.h
 *
 *  Created on: 25.01.2017
 *      Author: tobias
 */

#ifndef HELPER_SETTINGS_INITIATORSETTINGS_H_
#define HELPER_SETTINGS_INITIATORSETTINGS_H_

#include "NodeBaseSettings.h"

namespace helper {
namespace settings {

class InitiatorSettings: public NodeBaseSettings {
public:
	InitiatorSettings(const string& filename);
	virtual ~InitiatorSettings();

	virtual network::TransceiverBase* getTransceiver();
};

} /* namespace settings */
} /* namespace helper */

#endif /* HELPER_SETTINGS_INITIATORSETTINGS_H_ */
