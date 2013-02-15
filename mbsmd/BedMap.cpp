#include "BedMap.h"
#include <sstream>
#include <boost/lexical_cast.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/algorithm/string.hpp>

namespace cqs {

BedStrandMap::BedStrandMap(void)
{}

BedStrandMap::~BedStrandMap(void)
{
	BedStrandMap::iterator iter = this->begin();
	BedStrandMap::iterator iterEnd = this->end();
	while(iter != iterEnd){
		iter->second->clear();
		delete iter->second;
		iter++;
	}
	clear();
}

void BedStrandMap::addItem(BedItem* item){
	BedItems* items = this->findBedItems(item->getStrand());
	if(items == NULL){
		items = new BedItems();
		(*this)[item->getStrand()] = items;
	}
	items->push_back(item);
}

BedItems* BedStrandMap::findBedItems(const char strand){
	BedStrandMap::iterator iter = this->find(strand);
	if(iter == this->end()){
		return NULL;
	}
	else{
		return iter->second;
	}
}

void BedStrandMap::fillSequence(Sequence* seq){
	BedStrandMap::iterator iter = this->begin();
	BedStrandMap::iterator iterEnd = this->end();
	while(iter != iterEnd){
		iter->second->fillSequence(seq);
		iter++;
	}
}

BedChromosomeMap::BedChromosomeMap(void)
{
}

BedChromosomeMap::~BedChromosomeMap(void)
{
	BedChromosomeMap::iterator iter = this->begin();
	BedChromosomeMap::iterator iterEnd = this->end();
	while(iter != iterEnd){
		delete iter->second;
		iter++;
	}
	clear();
}

void BedChromosomeMap::buildFromItems(BedItems* items){
	for (size_t i = 0; i < items->size(); i++) {
		BedItem* bitem = (*items)[i];

		BedChromosomeMap::iterator iter = this->find(bitem->getChrom());
		if(iter == this->end()){
			BedStrandMap* smap = new BedStrandMap();
			smap->addItem(bitem);
			(*this)[bitem->getChrom()] = smap;
		}
		else{
			iter->second->addItem(bitem);
		}
	}
}

BedItems* BedChromosomeMap::findBedItems(const string& chromosome, const char strand){
	BedChromosomeMap::iterator iter = this->find(chromosome);
	if(iter == this->end()){
		return NULL;
	}

	return iter->second->findBedItems(strand);
}

set<string> BedChromosomeMap::getChromosomes(){
	set<string> result;
	BedChromosomeMap::iterator iter = this->begin();
	BedChromosomeMap::iterator iterEnd = this->end();
	while(iter != iterEnd){
		result.insert(iter->first);
		iter++;
	}
	return result;
}

void BedChromosomeMap::fillSequence(Sequence* seq){
	BedStrandMap* map =	this->find(seq->getName())->second;
	if(map != NULL){
		map->fillSequence(seq);
	}
}

}