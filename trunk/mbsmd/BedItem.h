/*
 * BedItem.h
 *
 *  Created on: Nov 28, 2012
 *      Author: shengq1
 */

#ifndef BEDITEM_H_
#define BEDITEM_H_

#include "GtfTranscriptItem.h"
#include "MatchExon.h"
#include "Sequence.h"
#include <string>
#include <vector>

namespace cqs {

using namespace std;

class BedItem {
private:
	string chrom;
	long chromStart;
	long chromEnd;
	string name;
	double score;
	char strand;
	long expectStart;
	long expectEnd;
	vector<MatchExon*> exons;
	string directExpectSequence;
public:
	BedItem();
	virtual ~BedItem();

	long getLength();

	void setExpectLength(int length);

	bool isInExon();

	const string& getChrom() const {
		return chrom;
	}

	void setChrom(const string& chrom) {
		this->chrom = chrom;
	}

	long getChromEnd() const {
		return chromEnd;
	}

	void setChromEnd(long chromEnd) {
		this->chromEnd = chromEnd;
	}

	long getChromStart() const {
		return chromStart;
	}

	void setChromStart(long chromStart) {
		this->chromStart = chromStart;
	}

	long getExpectEnd() const {
		return expectEnd;
	}

	void setExpectEnd(long expectEnd) {
		this->expectEnd = expectEnd;
	}

	long getExpectStart() const {
		return expectStart;
	}

	void setExpectStart(long expectStart) {
		this->expectStart = expectStart;
	}

	const string& getName() const {
		return name;
	}

	void setName(const string& name) {
		this->name = name;
	}

	double getScore() const {
		return score;
	}

	void setScore(double score) {
		this->score = score;
	}

	char getStrand() const {
		return strand;
	}

	void setStrand(char strand) {
		this->strand = strand;
	}

	const vector<MatchExon*>& getExons() const {
		return exons;
	}

	void addExon(MatchExon* exon) {
		this->exons.push_back(exon);
	}

	void removeExon(int index) {
		this->exons.erase(this->exons.begin() + index);
	}

	const string& getDirectExpectSequence() const {
		return directExpectSequence;
	}

	void setDirectExpectSequence(const string& value) {
		this->directExpectSequence = value;
	}

	void mergeExon();

	void matchGtfTranscriptItem(GtfTranscriptItem* gitem);

	void fillSequence(Sequence* seq);
};

}
#endif /* BEDITEM_H_ */
