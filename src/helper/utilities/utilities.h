/*
 * utilities.h
 *
 *  Created on: 16.11.2016
 *      Author: tobias
 */

#ifndef HELPER_UTILITIES_UTILITIES_H_
#define HELPER_UTILITIES_UTILITIES_H_

#include <string>

namespace helper {
namespace utilities {

static inline bool isNumber(const std::string& str) {
	std::string::const_iterator it = str.begin();
	while (it != str.end() && std::isdigit(*it)) ++it;
	return !str.empty() && it == str.end();
}

}
}



#endif /* HELPER_UTILITIES_UTILITIES_H_ */
