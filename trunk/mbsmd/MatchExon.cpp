/*
 * MatchExon.cpp
 *
 *  Created on: Nov 30, 2012
 *      Author: shengq1
 */

#include "MatchExon.h"

namespace cqs {

MatchExon::MatchExon() {
	transcriptId = "";
	retainedIntron = false;
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

	for (int i = 0; i < this->size(); i++) {
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

	for (int i = 0; i < another->size(); i++) {
		Location* loci = (*another)[i];
		bool contained = false;
		for(int j = 0;j < this->size();j++){
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

} /* namespace cqs */

