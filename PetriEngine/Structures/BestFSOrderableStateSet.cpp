#include "BestFSOrderableStateSet.h"

namespace PetriEngine {
namespace Structures {

bool BestFSOrderableStateSet::add(State *state){
	// Ensure we're larger than any state in waiting. Pop any states smaller
	bool skipVisited = false;
	for(iter it = _waiting.begin(); it != _waiting.end();){
		if(this->less(it.item(), state)){
			_waiting.remove(it++);
			skipVisited = true;
		} else {
			if(this->leq(state, it.item()))
				return false;
			it++;
		}
	}

	// Ensure we haven't been visited, or are smaller than or
	//  equal any states in visited
	if(!skipVisited){
		for(liter it = _visited.begin(); it != _visited.end();){
			if(this->less(*it, state))
				_visited.remove(*it++);
			else {
				if(this->leq(state, *it))
					return false;
				it++;
			}
		}
	}

	// Calculate priority, and add
	PQL::DistanceContext context(*this->_net,
								 _distanceStrategy,
								 state->marking(),
								 state->intValuation(),
								 state->boolValuation(),
								 _dm);
	double d = _query->distance(context);

	_waiting.push(d, state);
	return true;
}

State* BestFSOrderableStateSet::getNextState(){
	State* tmp = _waiting.pop();
	_visited.push_back(tmp);
	return tmp;
}

size_t BestFSOrderableStateSet::waitingSize(){
	return _waiting.size();
}

} // Structures
} // PetriEngine
