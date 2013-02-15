//============================================================================
// Name        : mbsmd.cpp
// Author      : Quanhu Sheng, Qi Liu, Shyr Yu
// Version     :
// Copyright   : CQS/VUMC
//============================================================================

#include "GtfTranscriptItemFile.h"
#include "GtfItem.h"
#include "BedItem.h"
#include "BedItems.h"
#include "BedFile.h"
#include "MatchExon.h"
#include "BedMatchFile.h"
#include "FastaFile.h"
#include "Utils.h"
#include "BedMap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <map>
#include <algorithm>
#include <boost/lexical_cast.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace cqs;

int main(void) {
	//string bedfile = "H:/shengquanhu/projects/liuqi/msbmd/test.bed";
	//string gtffile = "H:/shengquanhu/projects/liuqi/msbmd/test.gtf";
	string bedfile = "H:/shengquanhu/projects/liuqi/msbmd/peaks.bed";
	string gtffile = "H:/shengquanhu/projects/liuqi/msbmd/Homo_sapiens.GRCh37.68.gtf";
	string fafile = "H:/shengquanhu/projects/liuqi/msbmd/hg19_chr.fa";
	int extends[6];
	extends[0] = 50;
	extends[1] = 75;
	extends[2] = 100;
	extends[3] = 200;
	extends[4] = 500;
	extends[5] = 1000;

	//int extends[1];
	//extends[0] = 50;

	char strands[2];
	strands[0] = '+';
	strands[1] = '-';

	string sfile = bedfile + ".stat";
	boost::scoped_ptr<ofstream> ss(new ofstream(sfile));
	if (!ss->is_open()) {
		cerr << "Cannot open file " << sfile << endl;
		cin.get();
		exit(0);
	}

	*ss
			<< "WindowSize\tHighQuality_SingleExon\tHighQuality_SingleExon_Percentage\tHighQuality_MultipleExons\tHighQuality_MultipleExons_Percentage\tLowQuality_Intron\tLowQuality_Intron_Percentage\tLowQuality_MultipleCandidate\tLowQuality_MultipleCandidate_Percentage\tUnmatched\tUnmatched_Percentage"
			<< endl;

	for (int e = 0; e < GetArrLength(extends); e++) {
		long extend = extends[e];

		string hfile = bedfile + "." + boost::lexical_cast < string
				> (extend) + ".highquality.match";
		string lfile = bedfile + "." + boost::lexical_cast < string
				> (extend) + ".lowquality.match";
		string ufile = bedfile + "." + boost::lexical_cast < string
				> (extend) + ".unmapped.match";

		boost::scoped_ptr<ofstream> os(new ofstream(hfile));
		if (!os->is_open()) {
			cerr << "Cannot open file " << hfile << endl;
			cin.get();
			exit(0);
		}

		boost::scoped_ptr<ofstream> osf(new ofstream(hfile + ".fa"));
		if (!osf->is_open()) {
			cerr << "Cannot open file " << hfile + ".fa" << endl;
			cin.get();
			exit(0);
		}

		boost::scoped_ptr<ofstream> osff(new ofstream(hfile + "_direct.fa"));
		if (!osff->is_open()) {
			cerr << "Cannot open file " << hfile + "_direct.fa" << endl;
			cin.get();
			exit(0);
		}

		boost::scoped_ptr<ofstream> ls(new ofstream(lfile));
		if (!ls->is_open()) {
			cerr << "Cannot open file " << lfile << endl;
			cin.get();
			exit(0);
		}

		boost::scoped_ptr<ofstream> lsf(new ofstream(lfile + ".fa"));
		if (!lsf->is_open()) {
			cerr << "Cannot open file " << lfile + ".fa" << endl;
			cin.get();
			exit(0);
		}

		boost::scoped_ptr<ofstream> lsff(new ofstream(lfile + "_direct.fa"));
		if (!lsff->is_open()) {
			cerr << "Cannot open file " << lfile + "_direct.fa" << endl;
			cin.get();
			exit(0);
		}

		boost::scoped_ptr<ofstream> us(new ofstream(ufile));
		if (!us->is_open()) {
			cerr << "Cannot open file " << ufile << endl;
			cin.get();
			exit(0);
		}

		boost::scoped_ptr<ofstream> usff(new ofstream(ufile + "_direct.fa"));
		if (!usff->is_open()) {
			cerr << "Cannot open file " << ufile + "_direct.fa" << endl;
			cin.get();
			exit(0);
		}

		cout << "reading bed file ..." << endl;
		BedItems* beds = BedFile::readBedFile(bedfile);
		beds->setExpectLength(extend);

		BedChromosomeMap* bedmap = new BedChromosomeMap();
		bedmap->buildFromItems(beds);

		GtfTranscriptItemFile* gfile = new GtfTranscriptItemFile();
		gfile->Open(gtffile);
		GtfTranscriptItem* gtItem;
		cout << "reading gtf file ..." << endl;
		long count = 0;
		while ((gtItem = gfile->next()) != NULL) {
			count++;
			if (count % 1000 == 0) {
				cout << count << endl;
			}

			BedItems* items = bedmap->findBedItems(gtItem->getChrom(), gtItem->getStrand());
			if (items == NULL) {
				delete gtItem;
				continue;
			}

			//to match with bed, set gtf as start from 0
			gtItem->setStartToZeroBased();

			for (size_t i = 0; i < items->size(); i++) {
				BedItem* bitem = (*items)[i];
				bitem->matchGtfTranscriptItem(gtItem);
			}

			delete gtItem;
		}

		delete gfile;
		for (size_t i = 0; i < beds->size(); i++) {
			BedItem* bitem = (*beds)[i];

			//if(bitem->getChromStart() == 73470225){
			//	BedMatchFile::writeItem(cout, bitem);
			//}

			bitem->mergeExon();

			//if(bitem->getChromStart() == 73470225){
			//	BedMatchFile::writeItem(cout, bitem);
			//}
		}

		set<string> chrs = bedmap->getChromosomes();
		cout << "reading sequence file ..." << endl;
		FastaFile* ff = new FastaFile();
		ff->open(fafile);
		Sequence* seq;
		while ((seq = ff->next(chrs)) != NULL) {
			cout << "processing chromosome " << seq->getName() << "..." << endl;
			bedmap->fillSequence(seq);
			delete seq;
		}
		delete ff;

		BedMatchFile::writeHeader(*os);
		BedMatchFile::writeHeader(*ls);
		BedFile::writeHeader(*us);

		int unmapped = 0;
		int high_single = 0;
		int high_multiple = 0;
		int low_intron = 0;
		int low_multiple = 0;

		cout << "writing result file ..." << endl;
		for (size_t i = 0; i < beds->size(); i++) {
			BedItem* bitem = (*beds)[i];

			//unmaped
			if (bitem->getExons().size() == 0) {
				BedFile::writeItem(*us, bitem);
				*usff << ">" << bitem->getName() << endl
					<< bitem->getDirectExpectSequence() << endl;
				unmapped++;
				continue;
			}

			//high-quality
			if (bitem->getExons().size() == 1
					&& (!bitem->getExons()[0]->isRetainedIntron())) {
				BedMatchFile::writeItem(*os, bitem);
				MatchExon* exon = bitem->getExons()[0];

				if (exon->size() == 1) {
					high_single++;
				} else {
					high_multiple++;
				}

				*osf << ">" << bitem->getName() << endl << exon->getSequence()
						<< endl;
				*osff << ">" << bitem->getName() << endl
					<< bitem->getDirectExpectSequence() << endl;

				continue;
			}

			//low quality
			BedMatchFile::writeItem(*ls, bitem);
			*lsf << ">" << bitem->getName() << endl
					<< bitem->getExons()[0]->getSequence() << endl;
			*lsff << ">" << bitem->getName() << endl
					<< bitem->getDirectExpectSequence() << endl;

			if (bitem->isInExon()) {
				low_multiple++;
			} else {
				low_intron++;
			}
			continue;
		}

		int totalcount = beds->size();
		delete beds;

		*ss << extend << "\t" << high_single << "\t"
				<< high_single * 100.0 / totalcount << "\t" << high_multiple << "\t"
				<< high_multiple * 100.0 / totalcount  << "\t" << low_intron << "\t"
				<< low_intron * 100.0 / totalcount  << "\t" << low_multiple << "\t"
				<< low_multiple * 100.0 / totalcount  << "\t" << unmapped << "\t"
				<< unmapped * 100.0 / totalcount << endl;
	}

	exit(1);
}
