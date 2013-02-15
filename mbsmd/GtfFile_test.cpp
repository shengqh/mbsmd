/*
 * GtfFile_test.cpp
 *
 *  Created on: Nov 27, 2012
 *      Author: shengq1
 */

#include "GtfFile.h"
#include "GtfTranscriptItemFile.h"
#include <gtest/gtest.h>
#include <iostream>

namespace cqs {

string filename = "D:/sqh/programs/vc/mbsmd/mbsmd/data/Homo_sapiens.GRCh37.68.slim.gtf";

TEST(GtfFileTest, NextTest) {
	GtfFile* file = new GtfFile();
	file->open(filename);
	GtfItem* item = file->next();
	ASSERT_STREQ("GL000213.1", item->getSeqname().c_str());
	ASSERT_STREQ("miRNA", item->getSource().c_str());
	ASSERT_STREQ("CDS", item->getFeature().c_str());
	ASSERT_EQ(104742, item->getStart());
	ASSERT_EQ(104817, item->getEnd());
	ASSERT_EQ(0, item->getScore());
	ASSERT_EQ('+', item->getStrand());
	ASSERT_EQ('.', item->getFrame());
	ASSERT_STREQ(" gene_id \"ENSG00000265283\"; transcript_id \"ENST00000578976\"; exon_number \"1\"; gene_name \"MIR3118-5\"; gene_biotype \"miRNA\"; transcript_name \"MIR3118-5-201\";", item->getGroup().c_str());
	delete item;
	delete file;
}

TEST(GtfFileTest, NextExonTest) {
	GtfFile* file = new GtfFile();
	file->open(filename);
	GtfItem* item = file->nextExon();
	ASSERT_STREQ("GL000213.1", item->getSeqname().c_str());
	ASSERT_STREQ("protein_coding", item->getSource().c_str());
	ASSERT_STREQ("exon", item->getFeature().c_str());
	ASSERT_EQ(138767, item->getStart());
	ASSERT_EQ(139339, item->getEnd());
	ASSERT_EQ(0, item->getScore());
	ASSERT_EQ('-', item->getStrand());
	ASSERT_EQ('.', item->getFrame());
	ASSERT_STREQ(" gene_id \"ENSG00000237375\"; transcript_id \"ENST00000327822\"; exon_number \"1\"; gene_name \"BX072566.1\"; gene_biotype \"protein_coding\"; transcript_name \"BX072566.1-201\";", item->getGroup().c_str());
	delete item;
	delete file;
}

TEST(GtfFileTest, NextTranscriptExonItemTest) {
	GtfTranscriptItemFile* file = new GtfTranscriptItemFile();
	file->Open(filename);
	GtfTranscriptItem* tr = file->next();
	ASSERT_EQ(11, tr->size());
	GtfItem* item = (*tr)[10];
	ASSERT_STREQ("GL000213.1", item->getSeqname().c_str());
	ASSERT_STREQ("protein_coding", item->getSource().c_str());
	ASSERT_STREQ("exon", item->getFeature().c_str());
	ASSERT_EQ(138767, item->getStart());
	ASSERT_EQ(139339, item->getEnd());
	ASSERT_EQ(0, item->getScore());
	ASSERT_EQ('-', item->getStrand());
	ASSERT_EQ('.', item->getFrame());
	ASSERT_STREQ(" gene_id \"ENSG00000237375\"; transcript_id \"ENST00000327822\"; exon_number \"1\"; gene_name \"BX072566.1\"; gene_biotype \"protein_coding\"; transcript_name \"BX072566.1-201\";", item->getGroup().c_str());

	GtfTranscriptItem* item2 = file->next();
	ASSERT_STRNE((*item2)[0]->getTranscriptId().c_str(), item->getTranscriptId().c_str());

	delete item;
	delete file;
}

} /* namespace cqs */

