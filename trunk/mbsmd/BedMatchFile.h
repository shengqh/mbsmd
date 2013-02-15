/*
 * BedMatchFile.h
 *
 *  Created on: Dec 6, 2012
 *      Author: shengq1
 */

#ifndef BEDMATCHFILE_H_
#define BEDMATCHFILE_H_

#include "AbstractFile.h"
#include "BedItem.h"

namespace cqs {

using namespace std;

class BedMatchFile : public AbstractFile{
public:
	BedMatchFile();
	virtual ~BedMatchFile();

	bool open(const string filename) override;
	BedItem* next();
	static void writeHeader(ostream& os);
	static void writeItem(ostream& is, const BedItem* item);
};

} /* namespace cqs */
#endif /* BEDMATCHFILE_H_ */
