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

vector<string>& Utils::Split2(const string &s, char delim, vector<string>& result) {
	stringstream ss(s);
	string item;
	while (getline(ss, item, delim)) {
		result.push_back(item);
	}
	return result;
}

vector<string> Utils::Split(const string &s, char delim) {
	vector<string> result;
	return Split2(s, delim, result);
}

string Utils::toPositiveStrand(const string& sequence){
	stringstream result;
	string::const_reverse_iterator iterEnd = sequence.rend();
	for (string::const_reverse_iterator iter =
			sequence.rbegin(); iter != iterEnd;
			iter++) {
		if (*iter == 'A') {
			result << 'T';
		} else if (*iter == 'T') {
			result << 'A';
		} else if (*iter == 'G') {
			result << 'C';
		} else if (*iter == 'C') {
			result << 'G';
		} else {
			cout << "bp = " << *iter << endl;
			result << *iter;
		}
	}
	return result.str();
}


} /* namespace cqs */
