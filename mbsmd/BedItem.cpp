/*
 * BedItem.cpp
 *
 *  Created on: Nov 28, 2012
 *      Author: shengq1
 */

#include "BedItem.h"

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
}

BedItem::~BedItem() {
	for (size_t i = 0; i < exons.size(); i++) {
		delete exons[i];
	}
	exons.clear();
}

void BedItem::setExpectLength(int length) {
	int before = (length - (this->chromEnd - this->chromStart + 1)) / 2;
	this->expectStart = this->chromStart - before;
	this->expectEnd = this->expectStart + length - 1;
}

void BedItem::mergeExon() {
	bool hasRetainedIntron = false;
	bool hasFullExon = false;
	for (int j = this->exons.size() - 1; j > 0; j--) {
		if(this->exons[j]->isRetainedIntron()){
			hasRetainedIntron = true;
		}
		else{
			hasFullExon = true;
		}
	}

	if(hasFullExon && hasRetainedIntron){
		for (int j = this->exons.size() - 1; j > 0; j--) {
			if(this->exons[j]->isRetainedIntron()){
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
}

}
