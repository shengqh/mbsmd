//============================================================================
// Name        : mbsmd.cpp
// Author      : Quanhu Sheng, Qi Liu, Shyr Yu
// Version     :
// Copyright   : CQS/VUMC
// Description : Hello World in C, Ansi-style
//============================================================================

#include "GtfTranscriptItemFile.h"
#include "GtfItem.h"
#include "BedItem.h"
#include "BedItems.h"
#include "BedFile.h"
#include "MatchExon.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <map>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace cqs;

int main(void) {
//	string bedfile = "D:/projects/msbmd/peaks.test.bed";
//	string gtffile = "D:/projects/msbmd/Homo_sapiens.GRCh37.68.test.gtf";
	string bedfile = "H:/shengquanhu/projects/liuqi/msbmd/peaks.bed";
	string gtffile = "H:/shengquanhu/projects/liuqi/msbmd/Homo_sapiens.GRCh37.68.gtf";
	int extends[6];
	extends[0] = 50;
	extends[1] = 75;
	extends[2] = 100;
	extends[3] = 200;
	extends[4] = 500;
	extends[5] = 1000;

	string sfile = bedfile + ".stat";
	ofstream ss(sfile);
	if (!ss.is_open()) {
		cerr << "Cannot open file " << sfile << endl;
		cin.get();
		exit(0);
	}

	ss << "WindowSize\tHighQuality_SingleExon\tHighQuality_SingleExon_Percentage\tHighQuality_MultipleExons\tHighQuality_MultipleExons_Percentage\tLowQuality_Intron\tLowQuality_Intron_Percentage\tLowQuality_MultipleCandidate\tLowQuality_MultipleCandidate_Percentage\tUnmatched\tUnmatched_Percentage" << endl;

	for (int e = 0; e < 6; e++) {
		long extend = extends[e];

		string hfile = bedfile + "." + boost::lexical_cast < string
				> (extend) + ".highquality.match";
		string lfile = bedfile + "." + boost::lexical_cast < string
				> (extend) + ".lowquality.match";
		string ufile = bedfile + "." + boost::lexical_cast < string
				> (extend) + ".unmapped.match";
		ofstream os(hfile);
		if (!os.is_open()) {
			cerr << "Cannot open file " << hfile << endl;
			ss.close();
			cin.get();
			exit(0);
		}

		ofstream ls(lfile);
		if (!ls.is_open()) {
			cerr << "Cannot open file " << lfile << endl;
			ss.close();
			os.close();
			cin.get();
			exit(0);
		}

		ofstream us(ufile);
		if (!us.is_open()) {
			cerr << "Cannot open file " << ufile << endl;
			ss.close();
			os.close();
			ls.close();
			cin.get();
			exit(0);
		}

		cout << "reading bed file ..." << endl;
		BedItems* beds = BedFile::readBedFile(bedfile);
		beds->setExpectLength(extend);

		map<string, map<char, vector<BedItem*>>> bedmap;
		map<string, map<char, vector<BedItem*>>> ::iterator it1;
		map<char, vector<BedItem*>>::iterator it2;

		for (size_t i = 0; i < beds->size(); i++) {
			BedItem* bitem = (*beds)[i];
			bedmap[bitem->getChrom()][bitem->getStrand()].push_back(bitem);
		}

		/*
		 for(size_t y = 0;y < beds.size();y++){
		 cout << (beds[y]->getChromEnd() - beds[y]->getChromStart() + 1) << endl;
		 }
		 */

		vector<BedItem*> finished;

		GtfTranscriptItemFile* gfile = new GtfTranscriptItemFile();
		gfile->Open(gtffile);
		GtfTranscriptItem* item;
		cout << "reading gtf file ..." << endl;
		long count = 0;
		while ((item = gfile->next()) != NULL) {
			count++;
			if (count % 1000 == 0) {
				cout << count << endl;
			}

			it1 = bedmap.find(item->getChrom());
			if (it1 == bedmap.end()) {
				delete item;
				continue;
			}

			it2 = it1->second.find(item->getStrand());
			if (it2 == it1->second.end()) {
				delete item;
				continue;
			}

			vector<BedItem*>& items = it2->second;
			for (size_t i = 0; i < items.size(); i++) {
				BedItem* bitem = items[i];
				int index = item->find(bitem->getChromStart(),
						bitem->getChromEnd());
				if (-1 == index) {
					continue;
				}

				GtfItem* gitem = (*item)[index];
				MatchExon* exon = new MatchExon();
				bitem->addExon(exon);
				exon->setTranscriptId(gitem->getTranscriptId());
				exon->setTranscriptCount(1);

				Location* ml = new Location();
				exon->push_back(ml);
				if (gitem->getStart() > bitem->getChromStart()) {
					//if the peak range is out of the exon range, that peak range may be potential exon which
					//is detected by RNASeq data, so just extend the peak range to expect length rather than
					//extend by prior exon range
					ml->setStart(bitem->getExpectStart());
					exon->setIntronSize(
							gitem->getStart() - bitem->getChromStart());
					exon->setRetainedIntron(true);
				} else if (gitem->getStart() <= bitem->getExpectStart()) {
					//the exon has enough base pair
					ml->setStart(bitem->getExpectStart());
				} else {
					//the exon has no enough base pair
					ml->setStart(gitem->getStart());
					long expectLength = bitem->getExpectStart()
							- ml->getStart();
					int curindex = index - 1;
					while (expectLength > 0 && curindex >= 0) {
						Location* lp = new Location();
						exon->insert(exon->begin(), lp);

						GtfItem* prior = (*item)[curindex];
						lp->setEnd(prior->getEnd());
						if (prior->getLength() >= expectLength) {
							lp->setStart(prior->getEnd() - expectLength + 1);
							break;
						}

						expectLength = expectLength - prior->getLength();
						curindex--;
					}
				}

				if (gitem->getEnd() < bitem->getChromEnd()) {
					ml->setEnd(bitem->getExpectEnd());
					exon->setIntronSize(bitem->getChromEnd() - gitem->getEnd());
					exon->setRetainedIntron(true);
				} else if (gitem->getEnd() >= bitem->getExpectEnd()) {
					ml->setEnd(bitem->getExpectEnd());
				} else {
					ml->setEnd(gitem->getEnd());
					long expectLength = bitem->getExpectEnd() - ml->getEnd();
					int curindex = index + 1;
					while (expectLength > 0 && curindex < item->size()) {
						Location* lp = new Location();
						exon->push_back(lp);

						GtfItem* next = (*item)[curindex];
						lp->setStart(next->getStart());
						if (next->getLength() >= expectLength) {
							lp->setEnd(next->getStart() + expectLength - 1);
							break;
						} else {
							lp->setEnd(next->getEnd());
						}

						expectLength = expectLength - next->getLength();
						curindex++;
					}
				}
			}

			delete item;
		}

		delete item;
		delete gfile;
		for (size_t i = 0; i < beds->size(); i++) {
			BedItem* bitem = (*beds)[i];
			bitem->mergeExon();
		}

		os
				<< "chrom\tstart\tend\tname\tscore\tstrand\tmatch_count\ttranscriptid\ttranscript_count\tintron_retained\tintron_size\texon_count\texons..."
				<< endl;
		ls
				<< "chrom\tstart\tend\tname\tscore\tstrand\tmatch_count\ttranscriptid\ttranscript_count\tintron_retained\tintron_size\texon_count\texons..."
				<< endl;
		us << "chrom\tstart\tend\tname\tscore\tstrand" << endl;

		int unmapped = 0;
		int high_single = 0;
		int high_multiple = 0;
		int low_intron = 0;
		int low_multiple = 0;

		for (size_t i = 0; i < beds->size(); i++) {
			BedItem* bitem = (*beds)[i];

			//unmaped
			if (bitem->getExons().size() == 0) {
				us << bitem->getChrom() << "\t" << bitem->getChromStart()
						<< "\t" << bitem->getChromEnd() << "\t"
						<< bitem->getName() << "\t" << bitem->getScore() << "\t"
						<< bitem->getStrand() << endl;
				unmapped++;
				continue;
			}

			//high-quality
			if (bitem->getExons().size() == 1
					&& (!bitem->getExons()[0]->isRetainedIntron())) {
				MatchExon* exon = bitem->getExons()[0];
				os << bitem->getChrom() << "\t" << bitem->getChromStart()
						<< "\t" << bitem->getChromEnd() << "\t"
						<< bitem->getName() << "\t" << bitem->getScore() << "\t"
						<< bitem->getStrand() << "\t"
						<< bitem->getExons().size() << "\t"
						<< exon->getTranscriptId() << "\t"
						<< exon->getTranscriptCount() << "\t"
						<< exon->isRetainedIntron() << "\t"
						<< exon->getIntronSize() << "\t" << exon->size();
				for (size_t j = 0; j < exon->size(); j++) {
					os << "\t" << (*exon)[j]->getStart() << "-"
							<< (*exon)[j]->getEnd();
				}
				os << endl;

				if(exon->size() == 1){
					high_single++;
				}
				else{
					high_multiple++;
				}
				continue;
			}

			//low quality
			bool isIntron = true;
			for (size_t j = 0; j < bitem->getExons().size(); j++) {
				MatchExon* exon = bitem->getExons()[j];
				if(!exon->isRetainedIntron()){
					isIntron = false;
				}
				if (j == 0) {
					ls << bitem->getChrom() << "\t" << bitem->getChromStart()
							<< "\t" << bitem->getChromEnd() << "\t"
							<< bitem->getName() << "\t" << bitem->getScore()
							<< "\t" << bitem->getStrand() << "\t"
							<< bitem->getExons().size();
				} else {
					ls << "\t\t\t\t\t\t";
				}

				ls << "\t" << exon->getTranscriptId() << "\t"
						<< exon->getTranscriptCount() << "\t"
						<< exon->isRetainedIntron() << "\t"
						<< exon->getIntronSize() << "\t" << exon->size();
				for (size_t j = 0; j < exon->size(); j++) {
					ls << "\t" << (*exon)[j]->getStart() << "-"
							<< (*exon)[j]->getEnd();
				}
				ls << endl;
			}
			if(isIntron){
				low_intron ++;
			}
			else{
				low_multiple++;
			}
			continue;
		}

		int totalcount = beds->size();
		delete beds;
		os.close();
		ls.close();
		us.close();

		ss << extend << "\t" << high_single << "\t" << high_single * 100.0 / totalcount
			<< high_multiple << "\t" << high_multiple * 100.0 / totalcount
			<< low_intron << "\t" << low_intron * 100.0 / totalcount
			<< low_multiple << "\t" << low_multiple * 100.0 / totalcount
			<< unmapped << "\t" << unmapped * 100.0 / totalcount << endl;
	}

	ss.close();
	exit(1);
}
