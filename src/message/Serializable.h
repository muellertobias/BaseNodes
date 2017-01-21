/*
 * Serializable.h
 *
 *  Created on: 08.11.2016
 *      Author: tobias
 */

#ifndef MESSAGE_SERIALIZABLE_H_
#define MESSAGE_SERIALIZABLE_H_

#include "../helper/utilities/tinyxml2.h"
#include <string>

namespace network {

class Serializable {
public:
	virtual ~Serializable() {}
	virtual bool read(const std::string& str) = 0;
	virtual std::string write() const = 0;
	virtual tinyxml2::XMLElement* writeXMLElement(tinyxml2::XMLDocument& doc) const = 0;
};

}


#endif /* MESSAGE_SERIALIZABLE_H_ */
