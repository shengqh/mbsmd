/*
 * GtfItem.h
 *
 *  Created on: Nov 27, 2012
 *      Author: shengq1
 */

#ifndef GTFITEM_H_
#define GTFITEM_H_

#include <string>

namespace cqs {

using namespace std;

class GtfItem {
private:
	string seqname;
	string source;
	string feature;
	long start;
	long end;
	double score;
	char strand;
	char frame;
	string group;
	int exonNumber;
	string transcriptId;
public:
	GtfItem();

	void parseGroup();
	bool isSameTranscript(GtfItem* another);
	long getLength();
	bool inRange(long position);

	long getEnd() const {
		return end;
	}

	void setEnd(long end) {
		this->end = end;
	}

	int getExonNumber() const {
		return exonNumber;
	}

	void setExonNumber(int exonNumber) {
		this->exonNumber = exonNumber;
	}

	const string& getFeature() const {
		return feature;
	}

	void setFeature(const string& feature) {
		this->feature = feature;
	}

	char getFrame() const {
		return frame;
	}

	void setFrame(char frame) {
		this->frame = frame;
	}

	const string& getGroup() const {
		return group;
	}

	void setGroup(const string& group) {
		this->group = group;
		this->transcriptId = "";
		this->exonNumber = -1;
	}

	double getScore() const {
		return score;
	}

	void setScore(double score) {
		this->score = score;
	}

	const string& getSeqname() const {
		return seqname;
	}

	void setSeqname(const string& seqname) {
		this->seqname = seqname;
	}

	const string& getSource() const {
		return source;
	}

	void setSource(const string& source) {
		this->source = source;
	}

	long getStart() const {
		return start;
	}

	void setStart(long start) {
		this->start = start;
	}

	char getStrand() const {
		return strand;
	}

	void setStrand(char strand) {
		this->strand = strand;
	}

	const string& getTranscriptId() const {
		return transcriptId;
	}

	void setTranscriptId(const string& transcriptId) {
		this->transcriptId = transcriptId;
	}
};

} /* namespace cqs */
#endif /* GTFITEM_H_ */
