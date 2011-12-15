#include "IndexedRFSStateSet.h"

namespace PetriEngine { namespace Structures {

bool IndexedRFSStateSet::add(State *state, unsigned int t){
	// Ensure we're larger than any state in waiting. Pop any states smaller
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

	// Search visited
	if(!skipVisited){
		int is = state->stateIndex(*_net);
		for(visitedIter it = _visited.begin(); it != _visited.end();){
			int iv = it->first;

			if(is <= iv){
				// Check we are not smaller then existing states
				if(leq(state, it->second))
					return false;
			} else {
				// Clean up any superfluous states in visit
				if(less(it->second, state)){
					it = _visited.erase(it);
					this->_countRemove++;
					continue;
				}
			}
			it++;
		}
	}

	_stack.push_back(RStep(state,_net));


	State* next = state;

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


	return true;
}


RStep IndexedRFSStateSet::getNextStep(){
	RStep retState = _stack.back();
	return retState;
}

size_t IndexedRFSStateSet::waitingSize(){
	return _stack.size();
}

RStep IndexedRFSStateSet::popWating(){
	RStep temp = _stack.back();
	_stack.pop_back();
	return temp;
}

} // Structures
} // PetriEngine
