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

const string INIT = "init";
const string RESET = "reset";
const string SHUTDOWN = "shutdown";
const string SNAPSHOT = "snapshot";
const string ECHO_SHUTDOWN = "echo shutdown";

}

#endif /* HELPER_CONSTANTS_H_ */
