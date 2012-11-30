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

using namespace std;
using namespace cqs;

int main(void) {
//	string bedfile = "D:/projects/msbmd/peaks.test.bed";
//	string gtffile = "D:/projects/msbmd/Homo_sapiens.GRCh37.68.test.gtf";
	string bedfile = "D:/projects/msbmd/peaks.bed";
	string gtffile = "D:/projects/msbmd/Homo_sapiens.GRCh37.68.gtf";
	int extend = 50;

	string resultfile = bedfile + ".match";
	ofstream os(resultfile);
	if(!os.is_open()){
		cerr << "Cannot open file " << resultfile << endl;
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

			Location* ml = new Location();
			exon->push_back(ml);
			if (gitem->getStart() > bitem->getChromStart()) {
				//if the peak range is out of the exon range, that peak range may be potential exon which
				//is detected by RNASeq data, so just extend the peak range to expect length rather than 
				//extend by prior exon range
				ml->setStart(bitem->getExpectStart());
				exon->setRetainedIntron(true);
			}else if (gitem->getStart() <= bitem->getExpectStart()) {
				//the exon has enough base pair
				ml->setStart(bitem->getExpectStart());
			} else {
				//the exon has no enough base pair
				ml->setStart(gitem->getStart());
				long expectLength = bitem->getExpectStart() - ml->getStart();
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
				exon->setRetainedIntron(true);
			}
			else if (gitem->getEnd() >= bitem->getExpectEnd()) {
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
					}
					else{
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

	os
			<< "chrom\tstart\tend\tname\tscore\tstrand\tmatch_count\ttranscriptid\tretained_intron\texon_count\texons..."
			<< endl;
	for (size_t i = 0; i < beds->size(); i++) {
		BedItem* bitem = (*beds)[i];
		bitem->mergeExon();

		for (size_t j = 0; j < bitem->getExons().size(); j++) {
			MatchExon* exon = bitem->getExons()[j];
			if (j == 0) {
				os << bitem->getChrom() << "\t" << bitem->getChromStart()
						<< "\t" << bitem->getChromEnd() << "\t"
						<< bitem->getName() << "\t" << bitem->getScore() << "\t"
						<< bitem->getStrand() << "\t"
						<< bitem->getExons().size();
			} else {
				os << "\t\t\t\t\t\t";
			}

			os << "\t" << exon->getTranscriptId() << "\t" << exon->isRetainedIntron() << "\t" << exon->size();
			for (size_t j = 0; j < exon->size(); j++) {
				os << "\t" << (*exon)[j]->getStart() << "-"
						<< (*exon)[j]->getEnd();
			}
			os << endl;
		}
	}
	os.close();
	delete beds;

	exit(1);
}
