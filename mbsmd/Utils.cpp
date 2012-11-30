/*
 * Utils.cpp
 *
 *  Created on: Nov 27, 2012
 *      Author: shengq1
 */

#include "Utils.h"
#include <iostream>
#include <sstream>

namespace cqs {

std::vector<std::string>& Utils::Split2(const std::string &s, char delim,
		std::vector<std::string>& result) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		result.push_back(item);
	}
	return result;
}

std::vector<std::string> Utils::Split(const std::string &s, char delim) {
	std::vector<std::string> result;
	return Split2(s, delim, result);
}

} /* namespace cqs */
