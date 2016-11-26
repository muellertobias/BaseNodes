/*
 * trim.h
 *
 *  Created on: 07.11.2016
 *      Author: tobias
 */

#ifndef HELPER_STRING_TRIM_H_
#define HELPER_STRING_TRIM_H_

#include <string>
#include <vector>
#include <iostream>

/*
 * Gibt alles zurück, was sich vor dem Delimiter befindet
 */
static inline std::string trimSuffix(const std::string& str, const std::string& delimiter) {
	size_t endPosition = str.find(delimiter);

	if (endPosition != std::string::npos) {
		return str.substr(0, endPosition);
	}
	return "";
}

static inline std::string trimPrefix(const std::string& str, const std::string& delimiter) {
	size_t startPosition = str.find(delimiter);

	if (startPosition != std::string::npos) {
		return str.substr(startPosition + delimiter.size());
	}
	return "";
}

static inline std::vector<std::string> split(const std::string& str, const std::string& delimiter) {
	std::string tmp(str);
	std::vector<std::string> splittedString;
	while (tmp.size() > 0) {
		std::string prefix = trimSuffix(tmp, delimiter);
		if (prefix.size() == 0) {
			splittedString.push_back(tmp);
			tmp.clear();
		} else {
			splittedString.push_back(prefix);
			tmp = trimPrefix(tmp, delimiter);
		}

	}

	return splittedString;
}

#endif /* HELPER_STRING_TRIM_H_ */
