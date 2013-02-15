/*
 * BedMatchFile.cpp
 *
 *  Created on: Dec 6, 2012
 *      Author: shengq1
 */

#include "BedMatchFile.h"
#include "Utils.h"

namespace cqs {

BedMatchFile::BedMatchFile() {
	// TODO Auto-generated constructor stub

}

BedMatchFile::~BedMatchFile() {
	// TODO Auto-generated destructor stub
}

bool BedMatchFile::open(const string filename){
	bool result = AbstractFile::open(filename);
	string line;
	std::getline(read, line);
	return result;
}

MatchExon* parseMatchExon(vector<string>& parts){
	MatchExon* mx =new MatchExon();
	mx->setTranscriptId(parts[7]);
	mx->setTranscriptType(parts[8]);
	mx->setTranscriptCount(atoi(parts[9].c_str()));
	mx->setRetainedIntron(parts[10] == "1");
	mx->setIntronSize(atoi(parts[11].c_str()));

	vector<string> exons;
	Utils::Split2(parts[13], ';', exons);
	vector<string>::iterator iter = exons.begin();
	vector<string>::iterator iterEnd = exons.end();
	while(iter != iterEnd){
		mx->push_back(Location::parse(*iter));
		iter++;
	}
	return mx;
}

BedItem* BedMatchFile::next() {
	string line = "";
	while (!read.eof()) {
		std::getline(read, line);
		if(line != "" && line[0] != '\t'){
			break;
		}
	}

	if(line == ""){
		return NULL;
	}

	vector<string> parts;
	Utils::Split2(line, '\t', parts);

	BedItem* item = new BedItem();
	item->setChrom(parts[0]);
	item->setChromStart(atol(parts[1].c_str()));
	item->setChromEnd(atol(parts[2].c_str()));
	item->setName(parts[3]);
	item->setScore(atof(parts[4].c_str()));
	item->setStrand(parts[5][0]);
	item->addExon(parseMatchExon(parts));

	while(!read.eof()){
		char c = read.peek();
		if(c != '\t'){
			break;
		}

		std::getline(read, line);
		parts.clear();
		Utils::Split2(line, '\t', parts);

		item->addExon(parseMatchExon(parts));
	}

	return item;
}

void BedMatchFile::writeHeader(ostream& os) {
	os
			<< "chrom\tstart\tend\tname\tscore\tstrand\tmatch_count\ttranscriptid\ttranscript_type\ttranscript_count\tintron_retained\tintron_size\texon_count\texons..."
			<< endl;
}

void BedMatchFile::writeItem(ostream& os, const BedItem* item) {
	for (size_t j = 0; j < item->getExons().size(); j++) {
		MatchExon* exon = item->getExons()[j];
		if (j == 0) {
			os << item->getChrom() << "\t" << item->getChromStart() << "\t"
					<< item->getChromEnd() << "\t" << item->getName() << "\t"
					<< item->getScore() << "\t" << item->getStrand() << "\t"
					<< item->getExons().size();
		} else {
			os << "\t\t\t\t\t\t";
		}

		os << "\t" << exon->getTranscriptId() << "\t"
			<< exon->getTranscriptType() << "\t"
				<< exon->getTranscriptCount() << "\t"
				<< exon->isRetainedIntron() << "\t" << exon->getIntronSize()
				<< "\t" << exon->size() << "\t";
		for (size_t j = 0; j < exon->size(); j++) {
			if(j != 0){
				os << ";";
			}
			os << (*exon)[j]->getStart() << "-" << (*exon)[j]->getEnd();
		}
		os << endl;
	}
}

} /* namespace cqs */
