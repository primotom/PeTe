#include "IndexedRFSStateSet.h"

namespace PetriEngine { namespace Structures {

bool IndexedRFSStateSet::add(State *state){
	// Ensure we're larger than any state in waiting. Pop any states smaller
	bool skipVisited = false;
	for(waitingIter it = _waiting.begin(); it != _waiting.end(); ){
		if(this->less(*it, state)){
			_waiting.remove(*it++);
			this->_countRemove++;
			skipVisited = true;
		} else {
			if(this->leq(state, *it))
				return false;
			it++;
		}
	}

	// Search visited
	if(!skipVisited){
		int is = state->stateIndex(*_net);
		for(visitedIter it = _visited.begin(); it != _visited.end();){
			int iv = it->first;

			if(is <= iv){
				// Check we are not smaller then existing states
				if(leq(state, it->second))
					return false;
			}
			it++;
		}
	}

	return true;
}

void IndexedRFSStateSet::pushToWaiting(State *state){
	_waiting.push_front(state);
}

State* IndexedRFSStateSet::getNextState(){
	if(_waiting.empty())
		return NULL;

	State* next = _waiting.front();
	_waiting.pop_front();

	int is = next->stateIndex(*_net);

	if(_visited.empty()){
		_visited.push_back(IndexedState(is, next));
		return next;
	}

	for(visitedIter it = _visited.begin(); it != _visited.end(); it++){
		int iv = it->first;

		if(is > iv){
			_visited.insert(it, IndexedState(is, next));
			return next;
		} else if(is == iv) {
			if(_variance) {
				if(it->second->stateVariation(*_net, it->first) >= next->stateVariation(*_net, is)){
					_visited.insert(it, IndexedState(is, next));
					return next;
				}
			} else {
				_visited.insert(it, IndexedState(is, next));
				return next;
			}
		}
	}

	_visited.push_back(IndexedState(is, next));
	return next;
}

size_t IndexedRFSStateSet::waitingSize(){
	return _waiting.size();
}

} // Structures
} // PetriEngine
