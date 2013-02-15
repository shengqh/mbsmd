/*
 * FastaFile.cpp
 *
 *  Created on: Dec 6, 2012
 *      Author: shengq1
 */

#include "FastaFile.h"
#include <stddef.h>
#include <sstream>

namespace cqs {

FastaFile::FastaFile() {
}

Sequence* FastaFile::next() {
	set<string> empty;
	return next(empty);
}

Sequence* FastaFile::next(const set<string>& lookup) {
	stringstream ss;
	string line;
	Sequence* result = NULL;
	char c;
	while (!read.eof()) {
		c = read.peek();
		if (c == '>') {
			if (result == NULL) {
				std::getline(read, line);
				string name = line.substr(1);
				string description = "";

				size_t pos = name.find_first_of(' ');
				if(pos == string::npos){
					pos = name.find_first_of('\t');
				}
				if(pos != string::npos){
					description = name.substr(pos+1);
					name = name.substr(0, pos);
				}

				if(lookup.empty() || (lookup.find(name) != lookup.end())){
					result = new Sequence();
					result->setName(name);
					result->setDescription(description);
				}
			} else {
				break;
			}
		} else {
			std::getline(read, line);
			if (result != NULL) {
				ss << line;
			}
		}
	}

	if (result != NULL) {
		result->setSequence(ss.str());
	}

	return result;
}

} /* namespace cqs */
