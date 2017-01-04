/*
 * Serializable.h
 *
 *  Created on: 08.11.2016
 *      Author: tobias
 */

#ifndef NETWORK_SERIALIZABLE_H_
#define NETWORK_SERIALIZABLE_H_

namespace network {

#include <string>

class Serializable {
public:
	virtual ~Serializable() {}
	virtual bool read(const std::string& str, bool isNative) = 0;
	virtual std::string write(bool native) const = 0;

	static constexpr const char* DELIMITER = "::";
	static constexpr const char* TOKEN = "|";
};

}


#endif /* NETWORK_SERIALIZABLE_H_ */
