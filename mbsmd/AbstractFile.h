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

	virtual bool open(const string filename);
	virtual void close();
	virtual void reset();
};

}

#endif /* ABSTRACTFILE_H_ */
