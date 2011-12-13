#include "RandomDFSStateset.h"

namespace PetriEngine { namespace Structures {


bool RandomDFSStateSet::add(State *state){
	// Ensure we're larger than any state in waiting. Pop any states smaller
	bool skipVisited = false;
	for(iter it = _waiting.begin(); it != _waiting.end(); ){
		if(this->less(*it, state)){
			_waiting.remove(*it++);
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
			if (less(*it, state)){
				_states.erase(it++);
				this->_countRemove++;
			}else{
				if(this->leq(state, *it))
					return false;
				it++;
			}
		}
	}


	return true;
}

void RandomDFSStateSet::pushToWating(State* state){
	_waiting.push_back(state);
}

State* RandomDFSStateSet::getNextState(){
	if(!_waiting.empty()){
		State* retState = _waiting.back();
		_waiting.pop_back();
		_visited.push_back(retState);
		return retState;
	} else return NULL;
}

size_t RandomDFSStateSet::waitingSize(){
	return _waiting.size();
}

}}
