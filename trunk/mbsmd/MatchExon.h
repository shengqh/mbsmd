/*
 * MatchExon.h
 *
 *  Created on: Nov 30, 2012
 *      Author: shengq1
 */

#ifndef MATCHEXON_H_
#define MATCHEXON_H_

#include <string>
#include <vector>

namespace cqs {

using namespace std;

class Location {
public:
	Location() {
		this->start = -1;
		this->end = -1;
	}

	Location(long start, long end) {
		this->start = start;
		this->end = end;
	}

	long getEnd() const {
		return end;
	}

	void setEnd(long end) {
		this->end = end;
	}

	long getStart() const {
		return start;
	}

	void setStart(long start) {
		this->start = start;
	}

private:
	long start;
	long end;
};

class MatchExon: public vector<Location*> {
private:
	string transcriptId;
	bool retainedIntron;
public:
	MatchExon();
	virtual ~MatchExon();

	const string& getTranscriptId() const {
		return transcriptId;
	}

	void setTranscriptId(const string& transcriptId) {
		this->transcriptId = transcriptId;
	}

	bool equalLocations(const MatchExon* another) const;

	bool containLocations(const MatchExon* another) const;

	bool isRetainedIntron() const {
		return retainedIntron;
	}

	void setRetainedIntron(bool retainedIntron) {
		this->retainedIntron = retainedIntron;
	}
};

} /* namespace cqs */
#endif /* MATCHEXON_H_ */
