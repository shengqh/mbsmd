/*
 * GtfItem.cpp
 *
 *  Created on: Nov 27, 2012
 *      Author: shengq1
 */

#include "GtfItem.h"
#include <cstdlib>

namespace cqs {

GtfItem::GtfItem() {
	seqname = "";
	source = "";
	feature = "";
	start = -1;
	end = -1;
	score = 0;
	strand = '.';
	frame = 'x';
	group = "";
	exonNumber = -1;
	transcriptId = "";
}

void GtfItem::parseGroup() {
	size_t ipos, istart, iend;

	ipos = this->group.find("transcript_id");
	istart = this->group.find('"', ipos);
	iend = this->group.find('"', istart + 1);
	this->transcriptId = this->group.substr(istart + 1, iend - istart - 1);

	ipos = this->group.find("exon_number", iend + 1);
	istart = this->group.find('"', ipos);
	iend = this->group.find('"', istart + 1);
	this->exonNumber = atoi(
			this->group.substr(istart + 1, iend - istart - 1).c_str());
}

bool GtfItem::isSameTranscript(GtfItem* another) {
	if (this->exonNumber == -1) {
		this->parseGroup();
	}

	if (another->exonNumber == -1) {
		another->parseGroup();
	}

	return (this->transcriptId == another->transcriptId);
}

long GtfItem::getLength() {
	return this->end - this->start + 1;
}

bool GtfItem::inRange(long position){
	return this->start <= position && position <= this->end;
}


} /* namespace cqs */
