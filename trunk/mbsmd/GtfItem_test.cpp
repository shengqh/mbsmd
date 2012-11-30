/*
 * GtfItem_test.cpp
 *
 *  Created on: Nov 28, 2012
 *      Author: shengq1
 */

#include "../mbsmd/GtfItem.h"
#include <gtest/gtest.h>
#include <iostream>

namespace cqs {

TEST(GtfItemTest, ParseGroupTest) {
	GtfItem* item = new GtfItem();
	item->setGroup(" gene_id \"ENSG00000237375\"; transcript_id \"ENST00000327822\"; exon_number \"2\"; gene_name \"BX072566.1\"; gene_biotype \"protein_coding\"; transcript_name \"BX072566.1-201\";");
	item->parseGroup();
	ASSERT_STREQ("ENST00000327822", item->getTranscriptId().c_str());
	ASSERT_EQ(2, item->getExonNumber());
	delete item;
}

TEST(GtfItemTest, IsSameTranscriptTest) {
	GtfItem* item = new GtfItem();
	GtfItem* item2 = new GtfItem();

	item->setGroup(" gene_id \"ENSG00000237375\"; transcript_id \"ENST00000327822\"; exon_number \"2\"; gene_name \"BX072566.1\"; gene_biotype \"protein_coding\"; transcript_name \"BX072566.1-201\";");
	item2->setGroup(" gene_id \"ENSG00000237375\"; transcript_id \"ENST00000327822\"; exon_number \"3\"; gene_name \"BX072566.1\"; gene_biotype \"protein_coding\"; transcript_name \"BX072566.1-201\";");
	ASSERT_TRUE(item->isSameTranscript(item2));
	ASSERT_TRUE(item2->isSameTranscript(item));

	item2->setGroup(" gene_id \"ENSG00000237375\"; transcript_id \"ENST000003278XX\"; exon_number \"3\"; gene_name \"BX072566.1\"; gene_biotype \"protein_coding\"; transcript_name \"BX072566.1-201\";");
	ASSERT_FALSE(item->isSameTranscript(item2));
	ASSERT_FALSE(item2->isSameTranscript(item));

	delete item;
	delete item2;
}

} /* namespace cqs */




