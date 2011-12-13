#include "IndexedBestFSStateSet.h"
#include <iostream>

namespace PetriEngine {
namespace Structures {

bool IndexedBestFSStateSet::add(State *state){
	// Ensure we're larger than any state in waiting. Pop any states smaller

	//std::cout<<"Checking waiting"<<std::endl;

	bool skipVisited = false;
	for(waitingIter it = _waiting.begin(); it != _waiting.end();){
		if(this->less(it.item(), state)){
			_waiting.remove(it++);
			skipVisited = true;
		} else {
			if(this->leq(state, it.item()))
				return false;
			it++;
		}
	}

	//std::cout<<"Doing visited check"<<std::endl;

	// Perform visited check
	if(!skipVisited){
		int is = state->stateIndex(*_net);
		for(visitIter it = _visited.begin(); it != _visited.end();){
			int iv = it->first;

			if(is <= iv){
				// Check we are not smaller then existing states
				if(leq(state, it->second))
					return false;
			} else {
				// Clean up any superflous states in visited
				if(less(it->second, state)){
					it = _visited.erase(it);
					this->_countRemove++;
					continue;
				}
			}
			it++;
		}
	}

	//std::cout<<"Doing heuristic distance"<<std::endl;

	// Calculate priority and add state
	PQL::DistanceContext context(*this->_net,
								 _distanceStrategy,
								 state->marking(),
								 state->intValuation(),
								 state->boolValuation(),
								 _dm);
	double d = _query->distance(context);

	//std::cout<<"Pushing state to waiting"<<std::endl;

	_waiting.push(d, state);
	return true;
}

State* IndexedBestFSStateSet::getNextState(){
	if(_waiting.size() == 0)
		return NULL;

	State* next = _waiting.pop();
	int is = next->stateIndex(*_net);

	if(_visited.empty()){
		_visited.push_back(IndexedState(is, next));
		return next;
	}

	for(visitIter it = _visited.begin(); it != _visited.end(); it++){
		int iv = it->first;

		if(is > iv){
			_visited.insert(it, IndexedState(is, next));
			return next;
		} else if(is == iv){
			if(_variance){
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

size_t IndexedBestFSStateSet::waitingSize(){
	return _waiting.size();
}

} // Structures
} // PetriEngine
