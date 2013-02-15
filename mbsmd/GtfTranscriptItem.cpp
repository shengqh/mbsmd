/*
 * GtfTranscriptItem.cpp
 *
 *  Created on: Nov 29, 2012
 *      Author: shengq1
 */

#include "GtfTranscriptItem.h"

namespace cqs {

GtfTranscriptItem::GtfTranscriptItem() {
}

GtfTranscriptItem::~GtfTranscriptItem() {
	size_t i;
	for (i = 0; i < this->size(); i++) {
		delete (*this)[i];
	}
}

bool GtfTranscriptItem::isSameTranscript(GtfItem* item) {
	if(this->size() == 0){
		return true;
	}
	else{
		return (*this)[0]->isSameTranscript(item);
	}
}

string GtfTranscriptItem::getTranscriptId() {
	if(this->size() > 0){
		return (*this)[0]->getTranscriptId();
	}
	return "";
}

string GtfTranscriptItem::getChrom(){
	if(this->size() > 0){
		return (*this)[0]->getSeqname();
	}
	return "";
}

int GtfTranscriptItem::findIndex(long start, long end) {
	if(this->size() == 0){
		return -1;
	}

	if((*this)[0]->getStart() > end){
		return -1;
	}

	if(this->back()->getEnd() < start){
		return -1;
	}

	for(size_t i = 0;i < this->size(); i++){
		GtfItem* item = (*this)[i];
		if(item->inRange(start) || item->inRange(end)){
			return (int)i;
		}
	}

	return -1;
}

char GtfTranscriptItem::getStrand() {
	if(this->size() > 0){
		return (*this)[0]->getStrand();
	}
	return '.';
}

void GtfTranscriptItem::setStartToZeroBased(){
	for (size_t i = 0; i < this->size(); i++) {
		(*this)[i]->setStart((*this)[i]->getStart() - 1);
		(*this)[i]->setEnd((*this)[i]->getEnd() - 1);
	}
}

}
