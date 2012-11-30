/*
 * BedFile.h
 *
 *  Created on: Nov 27, 2012
 *      Author: shengq1
 */

#ifndef BEDFILE_H_
#define BEDFILE_H_

#include "BedItem.h"
#include "BedItems.h"
#include "AbstractFile.h"

#include <vector>
#include <string>
#include <fstream>

namespace cqs {

using namespace std;

class BedFile: public AbstractFile {
public:
	BedFile();

	BedItem* next();

	static BedItems* readBedFile(const string filename);
};

} /* namespace cqs */
#endif /* BEDFILE_H_ */
