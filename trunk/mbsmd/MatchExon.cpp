/*
 * MatchExon.cpp
 *
 *  Created on: Nov 30, 2012
 *      Author: shengq1
 */

#include "MatchExon.h"
#include "Utils.h"
#include <sstream>
#include <boost/algorithm/string.hpp>

namespace cqs {

Location* Location::parse(const string& loc){
	vector<string> values;
	Utils::Split2(loc, '-', values);
	Location* result = new Location();
	result->setStart(atol(values[0].c_str()));
	result->setEnd(atol(values[1].c_str()));
	return result;
}


MatchExon::MatchExon() {
	transcriptCount = 0;
	transcriptId = "";
	retainedIntron = false;
	intronSize = 0;
}

MatchExon::~MatchExon() {
	for (size_t i = 0; i < this->size(); i++) {
		delete (*this)[i];
	}
	this->clear();
}

bool MatchExon::equalLocations(const MatchExon* another) const {
	if (this->size() != another->size()) {
		return false;
	}

	for (size_t i = 0; i < this->size(); i++) {
		if ((*this)[i]->getStart() != (*another)[i]->getStart()
				|| (*this)[i]->getEnd() != (*another)[i]->getEnd()) {
			return false;
		}
	}
	return true;
}

bool MatchExon::containLocations(const MatchExon* another) const{
	if (this->size() < another->size()) {
		return false;
	}

	for (size_t i = 0; i < another->size(); i++) {
		Location* loci = (*another)[i];
		bool contained = false;
		for(size_t j = 0;j < this->size();j++){
			Location* locj = (*this)[j];
			if(locj->getStart() <= loci->getStart() && locj->getEnd() >= loci->getEnd()){
				contained = true;
				break;
			}
		}
		if(!contained){
			return false;
		}
	}

	return true;
}

void MatchExon::fillSequence(Sequence* seq, char strand){
	stringstream ss;
	for (size_t l = 0; l < this->size(); l++) {
		Location* loc = (*this)[l];
		ss	<< seq->getSequence().substr(loc->getStart(), loc->length());
	}
	
	string seqstr = ss.str();
	boost::to_upper(seqstr);

	if (strand == '-') {
		seqstr = Utils::toPositiveStrand(seqstr);
	}
	this->setSequence(seqstr);
}

bool MatchExonComparison(const MatchExon* e1, const MatchExon* e2){
	return e1->getTranscriptCount() > e2->getTranscriptCount();
}

} /* namespace cqs */

