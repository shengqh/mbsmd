/*
 * MatchExon_test.cpp
 *
 *  Created on: Nov 29, 2012
 *      Author: shengq1
 */

#include "BedItem.h"
#include <gtest/gtest.h>

namespace cqs {

TEST(BedItemTest, MergeExonTest) {
	BedItem* item = new BedItem();

	MatchExon* e1 = new MatchExon();
	e1->setTranscriptId("E1");
	e1->push_back(new Location(1,10));
	e1->push_back(new Location(30,40));

	MatchExon* e2 = new MatchExon();
	e2->setTranscriptId("E2");
	e2->push_back(new Location(1,10));
	e2->push_back(new Location(30,40));

	MatchExon* e3 = new MatchExon();
	e3->setTranscriptId("E3");
	e3->push_back(new Location(30,39));

	item->addExon(e1);
	item->addExon(e2);
	item->addExon(e3);

	ASSERT_EQ(3, item->getExons().size());
	item->mergeExon();
	ASSERT_EQ(1, item->getExons().size());
	ASSERT_STREQ("E1;E2", item->getExons()[0]->getTranscriptId().c_str());

	delete item;
}

}
