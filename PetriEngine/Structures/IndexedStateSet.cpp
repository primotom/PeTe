#include "Structures/IndexedStateSet.h"
#include <iostream>


namespace PetriEngine { namespace Structures {

bool IndexedStateSet::add(State* state){
	int is = state->stateIndex(*_net);
	bool skipVisited = false;
	bool foundPos = false;
	iter insertPos = _waiting.end();

	for(iter it = _waiting.begin(); it != _waiting.end();){
		int iw = it->first;

		if(is < iw){
			if(less(state, it->second))
				return false;
		} else if(is > iw){
			if(!foundPos) {
				foundPos = true;
				insertPos = it;
			}

			if(leq(it->second, state)){
				if(it == insertPos) {insertPos++;}
				it = _waiting.erase(it);
				skipVisited = true;
				continue;
			}
		} else {
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
				if(leq(state, it->second))
					return false;
			} else {
				if(less(it->second, state)){
					it = _visited.erase(it);
					continue;
				}
			}
			it++;
		}
	}

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

bool IndexedStateSet::equal(State* s1, State* s2){
	for(uint i= 0; i<_net->numberOfPlaces(); i++)
		if(s1->marking()[i]!= s2->marking()[i])
			return false;
	for(uint i= 0; i<_net->numberOfBoolVariables(); i++)
		if(s1->boolValuation()[i]!= s2->boolValuation()[i])
			return false;
	return true;
}

int IndexedStateSet::waitingSize(){
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
