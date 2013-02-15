#ifndef BEDMAP_H_
#define BEDMAP_H_

#include "BedItem.h"
#include "BedItems.h"
#include "Sequence.h"

#include <map>
#include <set>

namespace cqs {

using namespace std;

class BedStrandMap: public map<char, BedItems*> {
public:
	BedStrandMap();
	virtual ~BedStrandMap();

	void addItem(BedItem* item);
	BedItems* findBedItems(const char strand);

	void fillSequence(Sequence* seq);
};

class BedChromosomeMap: public map<string, BedStrandMap*> {
public:
	BedChromosomeMap();
	virtual ~BedChromosomeMap();

	void buildFromItems(BedItems* items);

	BedItems* findBedItems(const string& chromosome, const char strand);

	set<string> getChromosomes();

	void fillSequence(Sequence* seq);
};

} /* namespace cqs */
#endif /* BEDMAP_H_ */
