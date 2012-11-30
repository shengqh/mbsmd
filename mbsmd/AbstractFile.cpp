/*
 * AbstractFile.cpp
 *
 *  Created on: Nov 28, 2012
 *      Author: shengq1
 */

#include "AbstractFile.h"

namespace cqs {

AbstractFile::AbstractFile() {
}

AbstractFile::~AbstractFile() {
	if (read.is_open()) {
		read.close();
	}
}

bool AbstractFile::Open(string filename) {
	read.open(filename.c_str());
	return read.is_open();
}

void AbstractFile::Close() {
	if (read.is_open()) {
		read.close();
	}
}

void AbstractFile::Reset() {
	read.seekg(0, ios_base::beg);
}

}
