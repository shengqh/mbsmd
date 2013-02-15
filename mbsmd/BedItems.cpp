/*
 * BedItems.cpp
 *
 *  Created on: Nov 30, 2012
 *      Author: shengq1
 */

#include "BedItems.h"

namespace cqs {

BedItems::BedItems() {
}

BedItems::~BedItems() {
	for (size_t i = 0; i < this->size(); i++) {
		delete (*this)[i];
	}
	this->clear();
}

void BedItems::setExpectLength(int length) {
	for (size_t i = 0; i < this->size(); i++) {
		(*this)[i]->setExpectLength(length);
	}
}

void BedItems::fillSequence(Sequence* seq){
		BedItems::iterator jiter = this->begin();
		BedItems::iterator jiterEnd = this->end();
		while(jiter != jiterEnd){
			(*jiter)->fillSequence(seq);
			jiter++;
		}
}


} /* namespace cqs */
