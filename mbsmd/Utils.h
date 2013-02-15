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

using namespace std;

class Utils {
public:
	static vector<string> &Split2(const string &s, char delim, vector<string> &result);

	static vector<string> Split(const string &s, char delim);

	static string toPositiveStrand(const string& sequence);
};

template<typename T, int size> int GetArrLength(T(&)[size]){return size;}

} /* namespace cqs */
#endif /* UTILS_H_ */
