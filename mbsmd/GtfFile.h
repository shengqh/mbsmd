/*
 * GtfFile.h
 *
 *  Created on: Nov 27, 2012
 *      Author: shengq1
 */

#ifndef GTFFILE_H_
#define GTFFILE_H_

#include "GtfItem.h"
#include "GtfTranscriptItem.h"
#include "AbstractFile.h"
#include <string>
#include <vector>
#include <fstream>

namespace cqs {

using namespace std;

class GtfFile: public AbstractFile {
private:
	GtfItem* parseItem(const vector<string>& parts);
public:
	GtfFile();

	virtual GtfItem* next();

	GtfItem* nextExon();
};

} /* namespace cqs */
#endif /* GTFFILE_H_ */
