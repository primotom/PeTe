#include "DFSIndexedStateSet.h"

#include "Structures/DFSIndexedStateSet.h"
#include <iostream>


namespace PetriEngine { namespace Structures {

bool DFSIndexedStateSet::add(State* state , unsigned int t){
	int is = state->stateIndex(*_net);
	bool skipVisited = false;

	for(std::list<Step>::iterator it = _stack.begin() ; it != _stack.end();){
		if (less((*it).state, state)){
			_stack.erase(it++);
			skipVisited = true;
		} else {
			if(this->leq(state, (*it).state))
				return false;
			it++;
		}
	}

	if(!skipVisited){
		for(iter it = _visited.begin(); it != _visited.end();){
			int iv = it->first;

			if(is <= iv){
				// Check we are not smaller then existing states
				if(leq(state, it->second))
					return false;
			} else {
				// Clean up any superflous states in visited
				if(less(it->second, state)){
					it = _visited.erase(it);
					continue;
				}
			}
			it++;
		}
	}
	if(!_stack.empty())
		_stack.back().t = t + 1;

	if(_mode == PetriEngine::Structures::ModeGraterBool && state->parent()){

		if(greaterBool(state, state->parent()))
			_stack.push_back(Step(state,0));
		else
			_stack.push_front(Step(state, 0));

	}else if(_mode == PetriEngine::Structures::ModeGraterState && state->parent()){

		if(greater(state, state->parent()))
			_stack.push_back(Step(state,0));
		else
			_stack.push_front(Step(state, 0));

	}else{
		_stack.push_back(Step(state,0));
	}


	IndexedState next = IndexedState(is, state);
	//_waiting.pop_front();

	if(_visited.empty()){
		_visited.push_back(next);
		return true;
	}


	for(iter it = _visited.begin(); it != _visited.end(); it++){
		int iv = it->first;

		if(next.first > iv){
			_visited.insert(it, next);
			return true;
		} else if(next.first == iv){
			if(_variance){
				if(it->second->stateVariation(*_net, it->first) >= next.second->stateVariation(*_net, next.first)){
					_visited.insert(it, next);
					return true;
				}
			} else {
				_visited.insert(it, next);
				return true;
			}
		}
	}

	_visited.push_back(next);
	return true;
}

bool DFSIndexedStateSet::equal(State* s1, State* s2){
	for(uint i= 0; i<_net->numberOfPlaces(); i++)
		if(s1->marking()[i]!= s2->marking()[i])
			return false;
	for(uint i= 0; i<_net->numberOfBoolVariables(); i++)
		if(s1->boolValuation()[i]!= s2->boolValuation()[i])
			return false;
	return true;
}

int DFSIndexedStateSet::waitingSize(){
	return _stack.size();
}

Step DFSIndexedStateSet::getNextStep(){
	Step retState = _stack.back();
	return retState;
}

Step DFSIndexedStateSet::popWating(){
	Step temp = _stack.back();
	_stack.pop_back();
	return temp;
}

void DFSIndexedStateSet::printVisited(){
	std::cerr<<"Printing visited. length: "<<_visited.size()<<std::endl;
	for(iter it = _visited.begin(); it != _visited.end(); it++){
		for(uint m = 0; m <_net->numberOfPlaces(); m++)
			std::cerr<<it->second->marking()[m]<<" ";
		for(uint b = 0; b < _net->numberOfBoolVariables(); b++)
			std::cerr<<it->second->boolValuation()[b]<<" ";
		std::cerr<<std::endl;
	}
}

}}
