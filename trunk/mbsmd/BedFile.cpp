/*
 * BedFile.cpp
 *
 *  Created on: Nov 27, 2012
 *      Author: shengq1
 */

#include "BedFile.h"
#include "Utils.h"
#include <fstream>
#include <exception>
#include <cstdlib>

namespace cqs {

BedFile::BedFile() {
}

BedItem* BedFile::next() {
	if (!read.eof()) {
		string line;
		std::getline(read, line);
		vector<string> parts = Utils::Split(line, '\t');
		if (parts.size() >= 6) {
			BedItem* item = new BedItem();
			item->setChrom(parts[0]);
			item->setChromStart(atol(parts[1].c_str()));
			item->setChromEnd(atol(parts[2].c_str()));
			item->setName(parts[3]);
			item->setScore(atof(parts[4].c_str()));
			item->setStrand(parts[5][0]);
			return item;
		}
	}
	return 0;
}

BedItems* BedFile::readBedFile(const string filename) {
	BedItems* result = new BedItems();

	BedFile* file = new BedFile();
	if(!file->Open(filename)){
		delete file;
		throw string("Cannot open file ") + filename;
	}

	BedItem* item;
	while((item = file->next()) != 0){
		result->push_back(item);
	}

	delete file;

	return result;
}

} /* namespace cqs */
