/*
 * GtfTranscriptItem_test.cpp
 *
 *  Created on: Nov 29, 2012
 *      Author: shengq1
 */

#include "../mbsmd/GtfTranscriptItem.h"
#include "../mbsmd/GtfTranscriptItemFile.h"
#include <gtest/gtest.h>

namespace cqs {

GtfTranscriptItem* readData(GtfTranscriptItem** next){
	string filename =
			"D:/sqh/programs/vc/mbsmd/mbsmd/data/Homo_sapiens.GRCh37.68.slim.gtf";

	GtfTranscriptItemFile* file = new GtfTranscriptItemFile();
	file->Open(filename);

	GtfTranscriptItem* result = file->next();
	*next = file->next();

	delete file;

	return result;
}

TEST(GtfTranscriptItemTest, IsSameTranscriptTest) {
	GtfTranscriptItem* next;
	GtfTranscriptItem* item = readData(&next);

	for (int i = 1; i < 11; i++) {
		ASSERT_TRUE(NULL != (*item)[i]);
		ASSERT_TRUE(item->isSameTranscript((*item)[i]));
	}

	ASSERT_FALSE(item->isSameTranscript((*next)[0]));

	delete item;
}

TEST(GtfTranscriptItemTest, FindTest) {
	GtfTranscriptItem* next;
	GtfTranscriptItem* item = readData(&next);
	int index = item->findIndex(134290,134380);
	ASSERT_TRUE(index >= 0);
	GtfItem* find = (*item)[index];
	ASSERT_EQ(2, find->getExonNumber()); 

	delete item;
	delete next;
}

}
