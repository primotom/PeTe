#include "Structures/IndexedBFSStateSet.h"
#include <iostream>

namespace PetriEngine { namespace Structures {

bool IndexedBFSStateSet::add(State *state){
	int is = state->stateIndex(*_net);
	bool skipVisited = false;

	//Search waiting
	for(iter it = _waiting.begin(); it != _waiting.end();){
		if(this->less(it->second, state)){
			_waiting.erase(it++);
			this->_countRemove++;
			skipVisited = true;
		} else {
			if(this->leq(state, it->second))
				return false;
			it++;
		}
	}

	//Search visited
	if(!skipVisited){
		for(iter it = _visited.begin(); it != _visited.end();){
			int iv = it->first;

			if(is <= iv){
				// Check we are not smaller then existing states
				if(leq(state, it->second))
					return false;
			} else {
				// Clean up any superfluous states in visited
				if(less(it->second, state)){
					it = _visited.erase(it);
					this->_countRemove++;
					continue;
				}
			}
			it++;
		}
	}

	_waiting.push_back(IndexedState(is, state));
	return true;
}

State* IndexedBFSStateSet::getNextState(){
	if(_waiting.empty())
		return NULL;
	IndexedState next = _waiting.front();
	_waiting.pop_front();

	if(_visited.empty()){
		_visited.push_back(next);
		return next.second;
	}

	for(iter it = _visited.begin(); it != _visited.end(); it++){
		int iv = it->first;

		if(next.first > iv){
			_visited.insert(it, next);
			return next.second;
		} else if(next.first == iv){
			if(_variance){
				if(it->second->stateVariation(*_net, it->first) >= next.second->stateVariation(*_net, next.first)){
					_visited.insert(it, next);
					return next.second;
				}
			} else {
				_visited.insert(it, next);
				return next.second;
			}
		}
	}

	_visited.push_back(next);
	return next.second;
}

size_t IndexedBFSStateSet::waitingSize(){
	return _waiting.size();
}

} // Structures
} // PetriEngine
