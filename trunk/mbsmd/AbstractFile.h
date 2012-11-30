/*
 * AbstractFile.h
 *
 *  Created on: Nov 28, 2012
 *      Author: shengq1
 */

#ifndef ABSTRACTFILE_H_
#define ABSTRACTFILE_H_

#include <string>
#include <fstream>

namespace cqs {

using namespace std;

class AbstractFile {
protected:
	ifstream read;
public:
	AbstractFile();
	virtual ~AbstractFile();

	virtual bool Open(const string filename);
	virtual void Close();
	virtual void Reset();
};

}

#endif /* ABSTRACTFILE_H_ */
