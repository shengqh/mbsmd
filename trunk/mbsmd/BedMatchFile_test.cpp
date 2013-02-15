/*
 * BedMatchFile_test.cpp
 *
 *  Created on: Dec 6, 2012
 *      Author: shengq1
 */

#include "BedMatchFile.h"
#include <gtest/gtest.h>

namespace cqs {

TEST(BedMatchFileTest, ReadFileTest) {
	string filename = "D:/sqh/programs/vc/mbsmd/mbsmd/data/match.tsv";
	BedMatchFile* file = new BedMatchFile();
	file->open(filename);
	BedItem* item = file->next();

	ASSERT_STREQ("2", item->getChrom().c_str());
	ASSERT_EQ(73470225, item->getChromStart());
	ASSERT_EQ(73470251, item->getChromEnd());
	ASSERT_STREQ("2-60", item->getName().c_str());
	ASSERT_EQ(1064, item->getScore());
	ASSERT_EQ('+', item->getStrand());

	ASSERT_EQ(3, item->getExons().size());

	ASSERT_STREQ("ENST00000469844;ENST00000540468;ENST00000539919;ENST00000258091;ENST00000461290;ENST00000399032;ENST00000409924;ENST00000537131;ENST00000473786", item->getExons()[0]->getTranscriptId().c_str());
	ASSERT_STREQ("processed_transcript", item->getExons()[0]->getTranscriptType().c_str());
	ASSERT_TRUE(item->getExons()[0]->isRetainedIntron());
	ASSERT_EQ(3, item->getExons()[0]->getIntronSize());
	ASSERT_EQ(2, item->getExons()[0]->size());
	ASSERT_EQ(73470189, (*(item->getExons()[0]))[0]->getStart() );
	ASSERT_EQ(73470256, (*(item->getExons()[0]))[0]->getEnd() );
	ASSERT_EQ(73471124, (*(item->getExons()[0]))[1]->getStart() );
	ASSERT_EQ(73471155, (*(item->getExons()[0]))[1]->getEnd() );

	ASSERT_STREQ("ENST00000471927;ENST00000480489", item->getExons()[1]->getTranscriptId().c_str());
	ASSERT_STREQ("processed_transcript", item->getExons()[1]->getTranscriptType().c_str());
	ASSERT_TRUE(item->getExons()[1]->isRetainedIntron());
	ASSERT_EQ(16, item->getExons()[1]->getIntronSize());
	ASSERT_EQ(1, item->getExons()[1]->size());
	ASSERT_EQ(73470189, (*(item->getExons()[1]))[0]->getStart() );
	ASSERT_EQ(73470288, (*(item->getExons()[1]))[0]->getEnd() );

	ASSERT_STREQ("ENST00000538797", item->getExons()[2]->getTranscriptId().c_str());
	ASSERT_STREQ("protein_coding", item->getExons()[2]->getTranscriptType().c_str());
	ASSERT_FALSE(item->getExons()[2]->isRetainedIntron());
	ASSERT_EQ(0, item->getExons()[2]->getIntronSize());
	ASSERT_EQ(2, item->getExons()[2]->size());
	ASSERT_EQ(73470189, (*(item->getExons()[2]))[0]->getStart() );
	ASSERT_EQ(73470256, (*(item->getExons()[2]))[0]->getEnd() );
	ASSERT_EQ(73471671, (*(item->getExons()[2]))[1]->getStart() );
	ASSERT_EQ(73471702, (*(item->getExons()[2]))[1]->getEnd() );

	delete item;

	item = file->next();
	ASSERT_TRUE(item == NULL);

	delete file;
}

} /* namespace cqs */
