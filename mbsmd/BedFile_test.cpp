/*
 * BedFile_test.cpp
 *
 *  Created on: Nov 27, 2012
 *      Author: shengq1
 */

#include "../mbsmd/BedFile.h"
#include <gtest/gtest.h>

namespace cqs {

TEST(BedFileTest, ReadFileTest) {
	string filename = "D:/sqh/programs/gcc/mbsmd/data/peaks.slim.bed";
	BedItems* items = BedFile::readBedFile(filename);
	ASSERT_EQ(11, items->size());
	ASSERT_STREQ("21", (*items)[0]->getChrom().c_str());
	ASSERT_EQ(9827125, (*items)[0]->getChromStart());
	ASSERT_EQ(9827151, (*items)[0]->getChromEnd());
	ASSERT_STREQ("21-1", (*items)[0]->getName().c_str());
	ASSERT_EQ(1182877.2, (*items)[0]->getScore());
	ASSERT_EQ('+', (*items)[0]->getStrand());

	ASSERT_STREQ("12", (*items)[10]->getChrom().c_str());
	ASSERT_EQ(6647087, (*items)[10]->getChromStart());
	ASSERT_EQ(6647113, (*items)[10]->getChromEnd());
	ASSERT_STREQ("12-4", (*items)[10]->getName().c_str());
	ASSERT_EQ(49330.1, (*items)[10]->getScore());
	ASSERT_EQ('-', (*items)[10]->getStrand());

	delete items;
}

} /* namespace cqs */
