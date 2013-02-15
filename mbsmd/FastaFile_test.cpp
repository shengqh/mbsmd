/*
 * BedFile_test.cpp
 *
 *  Created on: Nov 27, 2012
 *      Author: shengq1
 */

#include "FastaFile.h"
#include <gtest/gtest.h>

namespace cqs {

TEST(FastaFileTest, NextTest) {
	string filename = "D:/sqh/programs/vc/mbsmd/mbsmd/data/dna.fa";
	FastaFile* file = new FastaFile();
	file->open(filename);

	Sequence* s1 = file->next();
	ASSERT_STREQ("1", s1->getName().c_str());
	ASSERT_STREQ("des1", s1->getDescription().c_str());
	ASSERT_STREQ("ccctaaccctaaccctaaccctaaccctaacccctaaccctaaccctaaaccctaaaccctaaccctaaccctaaccctaaccctaaccccaaccccaacc", s1->getSequence().c_str());

	set<string> lookup;
	lookup.insert("3");
	Sequence* s3 = file->next(lookup);
	ASSERT_STREQ("3", s3->getName().c_str());
	ASSERT_STREQ("", s3->getDescription().c_str());
	ASSERT_STREQ("gcttgctcacggtgctgtgccagggcgccccctgctggcgactagggcaactgcagggctctcttgcttagagtggtggccagcgccccctgctggcgccggggcactgcagggccctcttgcttactgtatagtggtggcacgccgcctg", s3->getSequence().c_str());

	delete s1;
	delete s3;
	delete file;
}

} /* namespace cqs */
