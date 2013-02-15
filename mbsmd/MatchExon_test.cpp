/*
 * MatchExon_test.cpp
 *
 *  Created on: Nov 29, 2012
 *      Author: shengq1
 */

#include "MatchExon.h"
#include <gtest/gtest.h>

namespace cqs {

TEST(LocationTest, ParseTest) {
	Location* loc = Location::parse("17970729-17970782");
	ASSERT_EQ(17970729, loc->getStart());
	ASSERT_EQ(17970782, loc->getEnd());
	delete loc;
}

TEST(MatchExonTest, EqualLocationsTest) {
	MatchExon* e1 = new MatchExon();
	e1->push_back(new Location(1,10));
	e1->push_back(new Location(30,40));

	MatchExon* e2 = new MatchExon();
	e2->push_back(new Location(1,10));
	e2->push_back(new Location(30,40));

	MatchExon* e3 = new MatchExon();
	e3->push_back(new Location(1,11));
	e3->push_back(new Location(30,40));

	ASSERT_TRUE(e1->equalLocations(e2));
	ASSERT_FALSE(e1->equalLocations(e3));

	delete e1;
	delete e2;
	delete e3;
}

TEST(MatchExonTest, ContainLocationsTest) {
	MatchExon* e1 = new MatchExon();
	e1->push_back(new Location(1,10));
	e1->push_back(new Location(30,40));

	MatchExon* e2 = new MatchExon();
	e2->push_back(new Location(1,10));
	e2->push_back(new Location(30,40));

	MatchExon* e3 = new MatchExon();
	e3->push_back(new Location(1,11));
	e3->push_back(new Location(30,40));

	ASSERT_TRUE(e1->containLocations(e2));
	ASSERT_TRUE(e2->containLocations(e1));
	ASSERT_FALSE(e1->containLocations(e3));
	ASSERT_TRUE(e3->containLocations(e1));
}

}
