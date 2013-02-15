/*
 * Sequence.h
 *
 *  Created on: Dec 6, 2012
 *      Author: shengq1
 */

#ifndef SEQUENCE_H_
#define SEQUENCE_H_

#include <string>

namespace cqs {

using namespace std;

class Sequence {
private:
	string name;
	string description;
	string sequence;
public:
	Sequence();
	virtual ~Sequence();

	const string& getName() const {
		return name;
	}

	void setName(const string& name) {
		this->name = name;
	}

	const string& getSequence() const {
		return sequence;
	}

	void setSequence(const string& sequence) {
		this->sequence = sequence;
	}

	const string& getDescription() const {
		return description;
	}

	void setDescription(const string& description) {
		this->description = description;
	}
};

} /* namespace cqs */

#endif /* SEQUENCE_H_ */