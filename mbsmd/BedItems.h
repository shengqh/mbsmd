/*
 * BedItems.h
 *
 *  Created on: Nov 30, 2012
 *      Author: shengq1
 */

#ifndef BEDITEMS_H_
#define BEDITEMS_H_

#include "BedItem.h"
#include <vector>

namespace cqs {

using namespace std;

class BedItems: public vector<BedItem*> {
public:
	BedItems();
	virtual ~BedItems();
	void setExpectLength(int length);
};

} /* namespace cqs */
#endif /* BEDITEMS_H_ */
