#include "Structures/IndexedStateSet.h"
#include <iostream>


namespace PetriEngine { namespace Structures {

bool IndexedStateSet::add(State* state){
	int is = state->stateIndex(*_net);
	bool skipVisited = false;
	bool foundPos = false;
	iter insertPos = _waiting.end();

	// Search waiting first
	for(iter it = _waiting.begin(); it != _waiting.end();){
		int iw = it->first;

		if(is < iw){
			// Index lower, ensure we're not smaller
			if(less(state, it->second))
				return false;
		} else if(is > iw){
			// Index greater, if we've not found our insert position yet, this is it
			if(!foundPos) {
				foundPos = true;
				insertPos = it;
			}

			// Check if state on waiting can be removed. If it can, we can skip Visited check
			if(leq(it->second, state)){
				if(it == insertPos) {insertPos++;}
				it = _waiting.erase(it);
				this->_countRemove++;
				skipVisited = true;
				continue;
			}
		} else {
			// Equal indices, check we are not equal, if not, check if we should insert here
			if(equal(it->second, state)) return false;
			else {
				if(_variance){
					if(it->second->stateVariation(*_net, iw) >= state->stateVariation(*_net, is) && !foundPos){
						insertPos = it;
						foundPos = true;
					}
				} else if(!foundPos) {
					insertPos = it;
					foundPos = true;
				}
			}
		}
		it++;
	}

	if(!skipVisited){
		for(iter it = _visited.begin(); it != _visited.end();){
			int iv = it->first;

			if(is <= iv){
				// Check we are not smaller then existing states
				if(leq(state, it->second))
					return false;
			}
			it++;
		}
	}

	// Push our state to waiting and return true
	if(insertPos == _waiting.end()){
		_waiting.push_back(IndexedState(is, state));
	} else {
		_waiting.insert(insertPos, IndexedState(is, state));
	}

	return true;
}

State* IndexedStateSet::getNextState(){
	if(_waiting.empty())
		return NULL;
	// Don't know which of these two to use
	//IndexedState next = _waiting.back();
	//_waiting.pop_back();
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

size_t IndexedStateSet::waitingSize(){
	return _waiting.size();
}

void IndexedStateSet::printWaiting(){
	std::cerr<<"Printing waiting. length: "<<_waiting.size()<<std::endl;
	for(iter it = _waiting.begin(); it != _waiting.end(); it++){
		for(uint m = 0; m <_net->numberOfPlaces(); m++)
			std::cerr<<it->second->marking()[m]<<" ";
		for(uint b = 0; b < _net->numberOfBoolVariables(); b++)
			std::cerr<<it->second->boolValuation()[b]<<" ";
		std::cout<<std::endl;
	}
}

void IndexedStateSet::printVisited(){
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
