/*
 * Echo.h
 *
 *  Created on: 22.01.2017
 *      Author: tobias
 */

#ifndef CORE_DATA_ECHO_H_
#define CORE_DATA_ECHO_H_

#include <map>

typedef struct {
	int EchoID;
	int FirstNeighborID;
	int counter;
} Echos;

typedef pair<int, Echos> EchoEntry;
typedef map<int, Echos> EchoBuffer;

#endif /* CORE_DATA_ECHO_H_ */
