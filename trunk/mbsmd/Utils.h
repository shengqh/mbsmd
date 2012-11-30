/*
 * Utils.h
 *
 *  Created on: Nov 27, 2012
 *      Author: shengq1
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <string>
#include <vector>

namespace cqs {

class Utils {
public:
	static std::vector<std::string> &Split2(const std::string &s, char delim,
			std::vector<std::string> &result);

	static std::vector<std::string> Split(const std::string &s, char delim);
};

} /* namespace cqs */
#endif /* UTILS_H_ */
