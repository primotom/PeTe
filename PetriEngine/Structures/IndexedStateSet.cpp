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
			if(leq(state, it->second))
				return false;
		} else if(is > iw){
			if(!foundPos) {
				foundPos = true;
				insertPos = it;
			}

			if(leq(it->second, state)){
				if(it == insertPos) insertPos++;
				_waiting.erase(it++);
				skipVisited = true;
				continue;
			}
		} else {
			if(equal(it->second, state)) return false;
			else {
				if(it->second->stateVariation(*_net, iw) >= state->stateVariation(*_net, is) && !foundPos){
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
					_visited.erase(it++);
					continue;
				}
			}
			it++;
		}
	}

	_waiting.insert(insertPos, IndexedState(is, state));

	/*int idx_s = state->stateIndex(*_net);
	bool bigger = false;
	bool lesser = false;
	bool insert = false;
	bool skipVisited = false;
	int i =0;
	iter insert_p;
	bool retFalse = false;
	//std::cerr<<"waiting "<<_waiting.size()<<std::endl;
	//std::cerr<<"visiting "<<_visited.size()<<std::endl;

	for(iter it = _waiting.begin(); it != _waiting.end();){
		//std::cerr<<"run "<<i<<". it "<<*it<<". waiting "<<*_waiting.end()<<std::endl;
		//std::cerr<<_waiting.empty()<<std::endl;
		i++;
		//std::cerr<<"done"<<std::endl;

		//std::cerr<<_waiting.size()<<std::endl;
		int idx_i = (*it)->stateIndex(*_net);

		if(idx_s < idx_i){
			lesser = true;
			if(leq(state, *it))
				return false;
			it++;
		}else if(idx_s > idx_i){
			bigger = true;
			if(lesser && !insert){
				_waiting.insert(it, state);
				insert_p = it;
				insert_p--;
				insert = true;
			}
			lesser = false;

			if(leq(*it, state)){
				_waiting.erase(it++);
				skipVisited = true;
			}
			else
				it++;
		}else{
			lesser = true;
			if(equal(*it, state))
				return false;
			else{
				if((*it)->stateVariation(*_net)>= state->stateVariation(*_net) && !insert){
					_waiting.insert(it, state);
					insert_p = it;
					insert_p--;
					insert = true;
				}
			}

			it++;
		}
	}

	//if(insert && skipVisited){
	/*if(!(insert && skipVisited)){//????????????????????
		std::cerr<<"size before "<<_waiting.size()<<std::endl;
		std::cerr<<"back "<<_waiting.back()<<std::endl;
		std::cerr<<"end "<<&_waiting.end()<<std::endl;
		std::cerr<<"insert "<<&insert_p<<std::endl;

		//_waiting.insert(insert_p, state);
		_waiting.erase(insert_p);
		std::cerr<<"size after "<<_waiting.size()<<std::endl;
	}/
	if(insert && skipVisited)
		return true;
	int df = 0;
	int j = 0;
	for(iter it = _visited.begin(); it != _visited.end();){
		//std::cerr<<"Visit: "<<df<<std::endl;
		df++;
		int idx_i = (*it)->stateIndex(*_net);

		if(idx_s < idx_i){
			if(leq(state, *it)){
				retFalse = true;
				break;
			}
			it++;
		} else if(idx_s > idx_i){
			j++;
			if(leq(*it, state)){
				_visited.erase(it++);
			}
			else
				it++;
		} else{
			if(equal(state, *it)){
				retFalse = true;
				break;
			}
			it++;
		}
	}

	//std::cerr<<"Removed from visited: "<<j<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1111"<<std::endl;
	if(retFalse && insert){
		_waiting.erase(insert_p);
		//std::cerr<<"Return false"<<std::endl;
		return false;
	}

	if((lesser && !bigger && !insert) || _waiting.empty())
		_waiting.push_back(state);
	else if(bigger && !lesser && !insert)
		_waiting.push_front(state);
	else if(!insert)//TODO: check this
		_waiting.insert(insert_p, state);
	return true;
*/
}

State* IndexedStateSet::getNextState(){
	if(_waiting.empty())
		return NULL;
	//IndexedState next = _waiting.back();
	//_waiting.pop_back();
	IndexedState next = _waiting.front();
	_waiting.pop_front();

	for(iter it = _visited.begin(); it != _visited.end(); it++){
		int iv = it->first;

		if(next.first > iv){
			_visited.insert(it, next);
			return next.second;
		} else if(next.first == iv){
			if(it->second->stateVariation(*_net, it->first) >= next.second->stateVariation(*_net, next.first)){
				_visited.insert(it, next);
				return next.second;
			}
		}
	}

	_visited.push_back(next);
	return next.second;

	/*
	State* next = _waiting.back();//TODO: Think about these two
	_waiting.pop_back();

	int idx_s = next->stateIndex(*_net);
	bool bigger = false;
	bool lesser = false;

	int i = 0;
	//std::cerr<<"start!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1"<<std::endl;
	for(iter it = _visited.begin(); it != _visited.end();it++){
		//std::cerr<<i<<std::endl;
		//i++;
		int idx_i = (*it)->stateIndex(*_net);

		if(idx_s < idx_i)
			lesser = true;
		else if(idx_s > idx_i){
			bigger = true;
			if(lesser){
				_visited.insert(it, next);
				return next;
			}
			lesser = false;
		}else{
			lesser = true;
			if((*it)->stateVariation(*_net) >= next->stateVariation(*_net)){
				_visited.insert(it, next);
				return next;
			}
		}
	}
	if((lesser && !bigger) || _visited.empty())
		_visited.push_back(next);
	else if(bigger && !lesser)
		_visited.push_front(next);
	return next;
	*/
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
	std::cerr<<"Printing waiting. lenght: "<<_waiting.size()<<std::endl;
	for(iter it = _waiting.begin(); it != _waiting.end(); it++){
		for(uint m = 0; m <_net->numberOfPlaces(); m++)
			std::cerr<<it->second->marking()[m]<<" ";
		for(uint b = 0; b < _net->numberOfBoolVariables(); b++)
			std::cerr<<it->second->boolValuation()[b]<<" ";
		std::cerr<<std::endl;
	}
}

void IndexedStateSet::printVisited(){
	std::cerr<<"Printing visited. lenght: "<<_visited.size()<<std::endl;
	for(iter it = _visited.begin(); it != _visited.end(); it++){
		for(uint m = 0; m <_net->numberOfPlaces(); m++)
			std::cerr<<it->second->marking()[m]<<" ";
		for(uint b = 0; b < _net->numberOfBoolVariables(); b++)
			std::cerr<<it->second->boolValuation()[b]<<" ";
		std::cerr<<std::endl;
	}
}

}}
