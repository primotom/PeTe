#include "BFSOrderableStateSet.h"
#include <iostream>

namespace PetriEngine { namespace Structures {

bool BFSOrderableStateSet::add(State *state){
	// Ensure we're larger than any state in waiting. Pop any states smaller
	bool skipVisited = false;
	for(iter it = _waiting.begin(); it != _waiting.end();){
		if(this->less(*it, state)){
			_waiting.erase(it++);
			skipVisited = true;
		} else {
			if(this->leq(state, *it))
				return false;
			it++;
		}
	}

	// Ensure we haven't been visited, or are smaller than or
	//  equal any states in visited
	if(!skipVisited){
		for(iter it = _visited.begin(); it != _visited.end();){
			if(this->leq(state, *it))
				return false;
			it++;
		}
	}

	_waiting.push_back(state);
	return true;
}

State* BFSOrderableStateSet::getNextState(){
	if(!_waiting.empty()){
		State* retState = _waiting.front();
		_waiting.pop_front();
		_visited.push_back(retState);
		return retState;
	} else return NULL;
}

size_t BFSOrderableStateSet::waitingSize(){
	return _waiting.size();
}

} // Structures
} // PetriEngine
