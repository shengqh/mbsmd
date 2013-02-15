/*
 * FastaFile.h
 *
 *  Created on: Dec 6, 2012
 *      Author: shengq1
 */

#ifndef FASTAFILE_H_
#define FASTAFILE_H_

#include "AbstractFile.h"
#include "Sequence.h"
#include <vector>
#include <string>
#include <fstream>
#include <set>

namespace cqs {

using namespace std;

class FastaFile : public AbstractFile {
public:
	FastaFile();
	Sequence* next();
	Sequence* next(const set<string>& lookup);
};

} /* namespace cqs */
#endif /* FASTAFILE_H_ */
