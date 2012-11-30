/*
* GtfFile.cpp
*
*  Created on: Nov 27, 2012
*      Author: shengq1
*/

#include "GtfFile.h"
#include "Utils.h"
#include <cstdlib>

namespace cqs {

	GtfFile::GtfFile() {
	}

	GtfItem* GtfFile::next() {
		if (!read.eof()) {
			string line;
			std::getline(read, line);
			vector<string> parts = Utils::Split(line, '\t');
			if (parts.size() >= 9) {
				return parseItem(parts);
			}
		}
		return NULL;
	}

	GtfItem* GtfFile::parseItem(const vector<string>& parts) {
		GtfItem* result = new GtfItem();
		result->setSeqname(parts[0]);
		result->setSource(parts[1]);
		result->setFeature(parts[2]);
		result->setStart(atoi(parts[3].c_str()));
		result->setEnd(atoi(parts[4].c_str()));
		if (parts[5] == ".") {
			result->setScore(0);
		} else {
			result->setScore(atof(parts[5].c_str()));
		}
		result->setStrand(parts[6][0]);
		result->setFrame(parts[7][0]);
		result->setGroup(parts[8]);
		return result;
	}

	GtfItem* GtfFile::nextExon() {
		while (!read.eof()) {
			string line;
			std::getline(read, line);
			vector<string> parts = Utils::Split(line, '\t');
			if (parts.size() >= 9 && parts[2] == "exon") {
				return parseItem(parts);
			}
		}
		return NULL;
	}
} /* namespace cqs */
