/*
 * GtfTranscriptItem.h
 *
 *  Created on: Nov 29, 2012
 *      Author: shengq1
 */

#ifndef GTFTRANSCRIPTITEM_H_
#define GTFTRANSCRIPTITEM_H_

#include "GtfItem.h"
#include <vector>
#include <string>

namespace cqs {

using namespace std;

class GtfTranscriptItem: public vector<GtfItem*> {
public:
	GtfTranscriptItem();
	virtual ~GtfTranscriptItem();

	string getChrom();
	bool isSameTranscript(GtfItem* item);
	string getTranscriptId();
	char getStrand();
	int findIndex(long start, long end);

	virtual void setStartToZeroBased();
};

}
#endif /* GTFTRANSCRIPTITEM_H_ */
