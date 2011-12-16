#include "RandomDFSStateset.h"

namespace PetriEngine { namespace Structures {

bool RandomDFSStateSet::add(State* state, unsigned int t){
	_countAdd++;

	bool skipVisited = false;
	for(std::list<RStep>::iterator it = _stack.begin() ; it != _stack.end();){
		if (less((*it).state, state)){
			_stack.erase(it++);
			this->_countRemove++;
			skipVisited = true;
		} else {
			if(this->leq(state, (*it).state))
				return false;
			it++;
		}
	}
	// Ensure we haven't been visited, or are smaller than or
	//  equal any states in visited
	if(!skipVisited){
		for(std::list<State*>::iterator it = _states.begin() ; it != _states.end();){
			if(leq(state,*it)){
				_countSkip++;
				return false;
			}
			it++;
		}
	}

	_stack.push_back(RStep(state,_net));

	_states.push_back(state);

	return true;
}


size_t RandomDFSStateSet::waitingSize(){
	return _stack.size();
}

RStep RandomDFSStateSet::getNextStep(){
	RStep retState = _stack.back();
	return retState;
}

RStep RandomDFSStateSet::popWating(){
	RStep temp = _stack.back();
	_stack.pop_back();
	return temp;
}

}}
