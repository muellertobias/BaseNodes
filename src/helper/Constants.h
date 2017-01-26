/*
 * Constants.h
 *
 *  Created on: 13.12.2016
 *      Author: tobias
 */

#ifndef HELPER_CONSTANTS_H_
#define HELPER_CONSTANTS_H_

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string>

#include "NodeInfo.h"

using namespace std;

namespace constants {

const string RESET = "reset";
const string SNAPSHOT = "snapshot";
const string SHUTDOWN = "shutdown";
const string SHUTDOWN_ALL = "shutdown all";
const string SHUTDOWN_ECHO = "shutdown echo";

}

#endif /* HELPER_CONSTANTS_H_ */
