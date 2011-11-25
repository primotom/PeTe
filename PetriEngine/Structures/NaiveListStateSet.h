#ifndef NAIVELISTSTATESET_H
#define NAIVELISTSTATESET_H

#include "PQL/Contexts.h"
#include <list>

namespace PetriEngine { namespace Structures {

class NaiveListStateSet {
public:
	NaiveListStateSet(){
	_countSkip =0;
	_countAdd =0;
	}

	bool add(State* state){
		_countAdd++;
		_states.push_back(std::make_pair(false,state));
		return true;

	}

	std::list<std::pair<bool,State*> > States() {return _states;}

	void writeStatistics(){
		std::cout<<"Number of states skipped: "<<_countSkip<<std::endl;
		std::cout<<"Number of states not skipped: "<<_countAdd-_countSkip<<std::endl;
		std::cout<<"Final size of visited and waiting: "<<_states.size()<<std::endl;
	}

private:
	std::list<std::pair<bool,State*> > _states;

	int _countSkip;
	int _countAdd;
};

}}

#endif // NAIVELISTSTATESET_H
