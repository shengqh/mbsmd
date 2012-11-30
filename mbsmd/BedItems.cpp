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

} /* namespace cqs */
