#include "GtfTranscriptItemFile.h"

namespace cqs {

GtfTranscriptItemFile::GtfTranscriptItemFile(void) {
	this->last = NULL;
	this->file = new GtfFile();
}

GtfTranscriptItemFile::~GtfTranscriptItemFile(void) {
	delete this->file;

	if (this->last != NULL) {
		delete this->last;
	}
}

bool GtfTranscriptItemFile::Open(string filename) {
	return this->file->open(filename);
}

void GtfTranscriptItemFile::Close() {
	this->file->close();
}

void GtfTranscriptItemFile::Reset() {
	this->file->reset();
}

GtfTranscriptItem* GtfTranscriptItemFile::next() {
	GtfTranscriptItem* result = new GtfTranscriptItem();
	if (this->last != NULL) {
		result->push_back(last);
	}

	while ((last = this->file->nextExon()) != NULL) {
		if (result->isSameTranscript(last)) {
			if (last->getStrand() == '-') {
				result->insert(result->begin(), last);
			} else {
				result->push_back(last);
			}
		} else {
			break;
		}
	}

	if (result->size() > 0) {
		return result;
	} else {
		delete result;
		return NULL;
	}
}

}
