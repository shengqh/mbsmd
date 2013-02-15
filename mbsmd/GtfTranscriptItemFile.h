#ifndef GTFTRANSCIPTITEMFILE_H_
#define GTFTRANSCIPTITEMFILE_H_

#include "GtfItem.h"
#include "GtfFile.h"
#include "GtfTranscriptItem.h"
#include <string>
#include <vector>
#include <fstream>

namespace cqs {

using namespace std;

class GtfTranscriptItemFile {
private:
	GtfFile* file;
	GtfItem* last;
public:
	GtfTranscriptItemFile(void);
	virtual ~GtfTranscriptItemFile(void);

	virtual bool Open(const string filename);
	virtual void Close();
	virtual void Reset();

	GtfTranscriptItem* next();
};

}

#endif /* GTFTRANSCIPTITEMFILE_H_ */

