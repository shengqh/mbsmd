/*
 * BedItem.cpp
 *
 *  Created on: Nov 28, 2012
 *      Author: shengq1
 */

#include "BedItem.h"
#include "Utils.h"
#include <algorithm>
#include <boost/algorithm/string.hpp>

namespace cqs {

BedItem::BedItem() {
	chrom = "";
	chromStart = -1;
	chromEnd = -1;
	name = "";
	score = 0;
	strand = '.';
	expectStart = -1;
	expectEnd = -1;
	directExpectSequence = "";
}

BedItem::~BedItem() {
	for (size_t i = 0; i < exons.size(); i++) {
		delete exons[i];
	}
	exons.clear();
}

long BedItem::getLength(){
	if(this->chromStart == -1 || this->chromEnd == -1){
		return 0;
	}
	else if(this->chromEnd > this->chromStart){
		return this->chromEnd - this->chromStart;
	}
	else{
		return this->chromStart - this->chromEnd;
	}
}

void BedItem::setExpectLength(int length) {
	int before = (length - this->getLength()) / 2;

	this->expectStart = this->chromStart - before;
	if(this->expectStart < 0){
		this->expectStart = 0;
	}

	this->expectEnd = this->expectStart + length - 1;
}

bool BedItem::isInExon() {
	if (this->exons.empty()) {
		return false;
	}

	for (size_t j = 0; j < this->exons.size(); j++) {
		MatchExon* exon = this->exons[j];
		if (exon->isRetainedIntron()) {
			return false;
		}
	}

	return true;
}

void BedItem::mergeExon() {
	bool hasRetainedIntron = false;
	bool hasFullExon = false;
	for (int j = this->exons.size() - 1; j >= 0; j--) {
		if (this->exons[j]->isRetainedIntron()) {
			hasRetainedIntron = true;
		} else {
			hasFullExon = true;
		}
	}

	if (hasFullExon && hasRetainedIntron) {
		for (int j = this->exons.size() - 1; j >= 0; j--) {
			if (this->exons[j]->isRetainedIntron()) {
				delete this->exons[j];
				this->removeExon(j);
			}
		}
	}

	for (int j = this->exons.size() - 1; j > 0; j--) {
		MatchExon* exonj = this->exons[j];
		for (int k = j - 1; k >= 0; k--) {
			MatchExon* exonk = this->exons[k];
			if (exonk->equalLocations(exonj)) {
				exonk->setTranscriptId(
						exonk->getTranscriptId() + ";"
								+ exonj->getTranscriptId());
				exonk->setTranscriptCount(
						exonk->getTranscriptCount()
								+ exonj->getTranscriptCount());
				delete exonj;
				this->removeExon(j);
				break;
			}

			if (exonk->containLocations(exonj)) {
				this->removeExon(j);
				delete exonj;
				break;
			}

			if (exonj->containLocations(exonk)) {
				this->exons[j] = exonk;
				this->exons[k] = exonj;
				this->removeExon(j);
				delete exonk;
				break;
			}
		}
	}

	sort(this->exons.begin(), this->exons.end(), MatchExonComparison);
}

void BedItem::matchGtfTranscriptItem(GtfTranscriptItem* gtItem){
	int index = gtItem->findIndex(this->getChromStart(), this->getChromEnd() - 1);
	if (-1 == index) {
		return;
	}

	GtfItem* gitem = (*gtItem)[index];
	MatchExon* exon = new MatchExon();
	this->addExon(exon);
	exon->setTranscriptId(gitem->getTranscriptId());
	exon->setTranscriptCount(1);
	exon->setTranscriptType(gitem->getSource());

	Location* ml = new Location();
	exon->push_back(ml);
	if (gitem->getStart() > this->getChromStart()) {
		//if the peak range is out of the exon range, that peak range may be potential exon which
		//is detected by RNASeq data, so just extend the peak range to expect length rather than
		//extend by prior exon range
		ml->setStart(this->getExpectStart());
		exon->setIntronSize(gitem->getStart() - this->getChromStart());
		exon->setRetainedIntron(true);
	} else if (gitem->getStart() <= this->getExpectStart()) {
		//the exon has enough base pair
		ml->setStart(this->getExpectStart());
	} else {
		//the exon has no enough base pair
		ml->setStart(gitem->getStart());
		long expectLength = ml->getStart() - this->getExpectStart();
		int curindex = index - 1;
		while (expectLength > 0 && curindex >= 0) {
			Location* lp = new Location();
			exon->insert(exon->begin(), lp);
			
			GtfItem* prior = (*gtItem)[curindex];
			lp->setEnd(prior->getEnd());
			if (prior->getLength() >= expectLength) {
				lp->setStart(prior->getEnd() - expectLength + 1);
				break;
			}
			else{
				lp->setStart(prior->getStart());
				expectLength = expectLength - prior->getLength();
				curindex--;
			}
		}
	}

	if (gitem->getEnd() < this->getChromEnd() - 1) {
		ml->setEnd(this->getExpectEnd());
		exon->setIntronSize(this->getChromEnd() - 1 - gitem->getEnd());
		exon->setRetainedIntron(true);
	} else if (gitem->getEnd() >= this->getExpectEnd()) {
		ml->setEnd(this->getExpectEnd());
	} else {
		ml->setEnd(gitem->getEnd());
		long expectLength = this->getExpectEnd() - ml->getEnd();
		int curindex = index + 1;
		while (expectLength > 0 && curindex < gtItem->size()) {
			Location* lp = new Location();
			exon->push_back(lp);

			GtfItem* next = (*gtItem)[curindex];
			lp->setStart(next->getStart());
			if (next->getLength() >= expectLength) {
				lp->setEnd(next->getStart() + expectLength - 1);
				break;
			} else {
				lp->setEnd(next->getEnd());
				expectLength = expectLength - next->getLength();
				curindex++;
			}
		}
	}
}

void BedItem::fillSequence(Sequence* seq){
	//matched exon fill sequence
	vector<MatchExon*>::iterator iter = this->exons.begin();
	vector<MatchExon*>::iterator iterEnd = this->exons.end();
	while(iter != iterEnd){
		(*iter)->fillSequence(seq, this->strand);
		iter++;
	}

	//fill direct sequence
	directExpectSequence = seq->getSequence().substr(this->expectStart, this->expectEnd - this->expectStart + 1);
	boost::to_upper(directExpectSequence);
	if(this->strand == '-'){
		directExpectSequence = Utils::toPositiveStrand(directExpectSequence);
	}
}

}
